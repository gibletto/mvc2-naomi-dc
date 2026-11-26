"""Statcheck for the port against a Flycast replay: each fight in the replay's SCRD archives (tools/fcreplay/
fcrun.py) is played again in the port from its start, with the same teams and the work Flycast had as the
entrances began, and both players' switches each game frame. Stops at the first frame the players, the fight
or the clock differ, and prints what.

  python -I tools/fcreplay/fcstat.py SCRDDIR OUTDIR [--fight=N] [--exe=mvc2.exe] [--lag=N] [--j=4]
      [--session[=FRAMES]]   one run from the select on, synced once
      [--write=FILE]         the (first) script written to FILE, the executable's addresses in it, and not run:
                             a replay to watch (FCR_NM: the executable's nm, FCR_BUILD: its build with arc.o)
"""
import glob, json, os, re, struct, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import scrd  # noqa: E402
import syms  # noqa: E402

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
DECOMP = os.environ.get("MVC2_DECOMP", os.path.dirname(ROOT))
BUILD = os.environ.get("FCR_BUILD", os.path.join(ROOT, "build-fcr"))
NM = os.environ.get("FCR_NM", "C:/msys64/mingw32/bin/nm.exe")
PLW = 0x5A4
NAMES = ["pl_work", "vs_sys_work", "sw_work", "sys_work", "sel_w", "nl_random_next", "work_buff", "rnd_hi", "stage_no"]
DUMP = [("pl_work", PLW * 6), ("vs_sys_work", 0xC0), ("sw_work", 0x28), ("sys_work", 0x100), ("rnd_hi", 8),
        ("nl_random_next", 4), ("stage_no", 4), ("sel_w", 0x58)]
JVS = [("start", 0x8000), ("up", 0x2000), ("down", 0x1000), ("left", 0x0800), ("right", 0x0400), ("lp", 0x0200),
       ("hp", 0x0100), ("a1", 0x0080), ("lk", 0x0040), ("hk", 0x0020), ("a2", 0x0010)]
# not the fight: the arcade's records and timers that the select and the system keep
IGNORE = {"route_no", "play_time", "score", "old_score", "old_score_hi", "beat_flag", "cpu_level_pts", "win_cnt",
          "win_max", "vs_record", "name", "name_flag", "best_rank", "score_rank", "win_rank", "card_open", "card_ex",
          "stage_cnt", "unk_55d"}


def naomi_syms():
    syms = {"rnd_hi": 0x0C283048}       # o_sub1.c's static, rnd_lo after it
    for line in open(os.path.join(DECOMP, "build", "mvc2.map")):
        m = re.match(r"\s*_(\w+)\s+H'([0-9A-F]{8})\s+DAT", line)
        if m and m.group(1) in NAMES:
            syms[m.group(1)] = int(m.group(2), 16)
    return syms


def port_syms(exe):
    """The arcade program's copies in the executable, told from the Dreamcast's by their distance from sys_work."""
    def table(path):
        t = {}
        for line in subprocess.run([NM, path], capture_output=True, text=True).stdout.splitlines():
            p = line.split()
            if len(p) == 3 and p[2].lstrip("_") in NAMES:
                t.setdefault(p[2].lstrip("_"), []).append(int(p[0], 16))
        return t
    arc, ex = table(os.path.join(BUILD, "arc.o")), table(exe)
    for base in ex["sys_work"]:
        syms = {}
        for n in NAMES:
            if n not in arc and len(ex.get(n, [])) == 1:
                syms[n] = ex[n][0]
                continue
            want = base + arc[n][0] - arc["sys_work"][0]
            if want in ex.get(n, []):
                syms[n] = want
        if len(syms) == len(NAMES):
            return syms
    sys.exit("port: the arcade's symbols not found")


def fields(header, struct_name, size):
    """A struct's fields from the decomp's header: [offset, size, name, kind] (kind p pointer, f float, i other)."""
    text = open(os.path.join(DECOMP, "include", header)).read()
    body = text[text.index("typedef struct %s {" % struct_name):text.index("} %s;" % struct_name)]
    out, inner = [], None
    for line in body.splitlines()[1:]:
        if re.match(r"\s*union\s*\{", line):    # a union: one field, a pointer if any member is
            inner = []
            continue
        if inner is not None:
            if not re.match(r"\s*\}", line):
                inner.append("*" in line.split(";")[0])
                continue
            m = re.match(r"\s*\}\s*(\w+)\s*;\s*/\*\s*([0-9a-f]{3})\b", line)
            out.append([int(m.group(2), 16), 0, m.group(1), "p" if any(inner) else "i"])
            inner = None
            continue
        m = re.match(r"\s*(.+?)\s*;\s*/\*\s*([0-9a-f]{3})\b", line)
        if not m:
            continue
        decl, off = m.group(1), int(m.group(2), 16)
        if "(" in decl:
            name = re.search(r"\(\s*\*+\s*(\w+)", decl).group(1)
        else:
            name = re.findall(r"(\w+)\s*(?:\[[^\]]*\])*$", decl)[0]
        kind = "p" if "*" in decl else "f" if re.match(r"(Float|VEC\d|MATRIX)\b", decl) else "i"
        if out and out[-1][0] == off:       # a union's members: one field, a pointer if any is
            if kind == "p":
                out[-1][3] = "p"
            continue
        out.append([off, 0, name, kind])
    out.sort()
    for i, f in enumerate(out):
        f[1] = (out[i + 1][0] if i + 1 < len(out) else size) - f[0]
    return out


PL_FIELDS = fields("pl_work.h", "PLAYER_WORK", PLW)
VS_FIELDS = fields("vs_sys.h", "VS_SYS_WORK", 0xC0)
SYS_FIELDS = fields("system.h", "SYSTEM_WORK", 0x100)
SEL_FIELDS = fields("select.h", "SEL_WORK", 0x58)


def keys(now):
    return "+".join(n for n, b in JVS if now & b) or "-"


class Fight:
    def __init__(self, n, start):
        self.n, self.start, self.frames = n, start, []     # frames: (vblank index, dumps by name) per game frame


def read_fights(scrd_dir, ns, want=None):
    """Each fight: from the first frame vs_st is 2, each game frame (the frame counter moved) until vs_st is 9 (the
    fight over: from there the game waits on loading, and Flycast's vblanks are no longer game frames)."""
    fights, cur, last_timer, n = [], None, None, 0
    i = 0
    for path in sorted(glob.glob(os.path.join(scrd_dir, "game_*.scrd")), key=lambda p: int(re.findall(r"\d+", p)[-1])):
        for head, ram in scrd.frames(path):
            r = scrd.Ram(ram)
            st = r.u(ns["vs_sys_work"], 1)
            timer = r.u(ns["sys_work"] + 0x1C)
            if cur is None and st == 2:
                cur = Fight(n, i)
                n += 1
                last_timer = None
            if cur is not None:
                if st in (0, 9) or st == 2 and cur.frames and cur.frames[-1][1]["vs_st"] > 2:
                    if want is None or cur.n == want:
                        fights.append(cur)
                    cur = None
                    if st == 2:
                        cur = Fight(n, i)
                        n += 1
                        last_timer = None
                elif timer != last_timer:
                    last_timer = timer
                    if want is None or cur.n == want:
                        d = {name: r.get(ns[name], size) for name, size in DUMP}
                        d["vs_st"], d["head"] = st, head
                        if not cur.frames:
                            d["globals"] = {g: r.get(arcade_addr(g), SYMS.size(g)) for g in GLOBALS}
                        cur.frames.append((i, d))
            i += 1
    if cur is not None and (want is None or cur.n == want):
        fights.append(cur)
    return fights


def read_session(scrd_dir, ns, limit):
    """The whole session as one run: from the first frame the select is ready (sel_w.ready_flag), each game
    frame, for at most limit game frames."""
    cur, last_timer, i = None, None, 0
    for path in sorted(glob.glob(os.path.join(scrd_dir, "game_*.scrd")), key=lambda p: int(re.findall(r"\d+", p)[-1])):
        for head, ram in scrd.frames(path):
            r = scrd.Ram(ram)
            if cur is None and r.u(ns["sel_w"] + 0xB, 1):
                cur = Fight(0, i)
            if cur is not None:
                timer = r.u(ns["sys_work"] + 0x1C)
                if timer != last_timer:
                    last_timer = timer
                    d = {name: r.get(ns[name], size) for name, size in DUMP}
                    d["vs_st"], d["head"] = r.u(ns["vs_sys_work"], 1), head
                    if not cur.frames:
                        d["globals"] = {g: r.get(arcade_addr(g), SYMS.size(g)) for g in GLOBALS}
                    cur.frames.append((i, d))
                    if len(cur.frames) >= limit:
                        return [cur]
            i += 1
    return [cur] if cur else []


def join_at(a, b):
    """The until condition if a session runs on after a load between frames a and b (each fight's entrances,
    each select's being ready), whose length differs in the port."""
    if b["vs_sys_work"][0] == 2 and a["vs_sys_work"][0] != 2:
        return "vs_sys_work+0 1 2"
    if b["sel_w"][0xB] and not a["sel_w"][0xB]:
        return "sel_w+0xb 1 %d" % b["sel_w"][0xB]
    return None


def joins(fight):
    """{game frame: until condition} for each join in the session."""
    f = [d for _, d in fight.frames]
    return dict((g, join_at(f[g - 1], f[g])) for g in range(1, len(f)) if join_at(f[g - 1], f[g]))


def segments(frames):
    """Each frame's count of joins so far."""
    out, n = [], 0
    for g, d in enumerate(frames):
        n += g > 0 and join_at(frames[g - 1], d) is not None
        out.append(n)
    return out


def timer0(d):
    return struct.unpack_from("<I", d["sys_work"], 0x1C)[0]


# the port's select is still closing for its first frames (Flycast's closed while it loaded), drawing from nlRand:
# its value is set again each frame till then
RNG_SETTLE = 10
SIZES = {"pl_work": PLW * 6, "vs_sys_work": 0xC0, "sw_work": 0x50, "sys_work": 0x100, "work_buff": 0x1D0 * 256}


SYMS = None     # syms.Symbols, made in main


def translate(ptr, ns, ps):
    """A pointer of the arcade's as the port's (syms.Symbols.translate)."""
    return SYMS.translate(ptr)


def pokes(name, base, flds, raw, ns, ps, skip=()):
    """Pokes for each field: the bytes Flycast had, and its pointers as the port's."""
    out = []
    for off, size, fname, kind in flds:
        if fname in skip:
            continue
        if fname == "tlocal":       # the character's own work: words, the arcade's pointers among them
            for at in range(0, size - 3, 4):
                v = struct.unpack_from("<I", raw, off + at)[0]
                if 0x0C000000 <= v < 0x0E000000:
                    v = translate(v, ns, ps)
                if v is not None:
                    out.append("poke %s+0x%x 4 0x%x" % (name, base + off + at, v))
            continue
        if kind == "p":
            for at in range(0, size - 3, 4):
                p = translate(struct.unpack_from("<I", raw, off + at)[0], ns, ps)
                if p is not None:
                    out.append("poke %s+0x%x 4 0x%x" % (name, base + off + at, p))
            continue
        b = raw[off:off + size]
        at = 0
        while at < size:
            n = 4 if size - at >= 4 and (off + at) % 4 == 0 else 2 if size - at >= 2 and (off + at) % 2 == 0 else 1
            out.append("poke %s+0x%x %d 0x%x" % (name, base + off + at, n, int.from_bytes(b[at:at + n], "little")))
            at += n
    return out


# the rest of the game's work that a fight runs on: the camera, the stage, the screen effects, and the
# effects' works and lists
GLOBALS = ["scr_work", "fog_work", "bright_work", "back_work", "stage_work", "fade_work", "flash_work", "light_work",
           "work_buff", "free_top", "free_end", "work_top", "work_end", "work_cnt", "work_rest_cnt", "free_cnt",
           "hit_work_w", "hit_work_cnt", "hit_work_old"]


def arcade_addr(name):
    return dict((n, a) for a, n in SYMS.arcade)[name]


def globals_copy(raw):
    """Pokes for GLOBALS as Flycast had them, a word at a time, pointers as the port's."""
    out, unknown = [], 0
    for name in GLOBALS:
        b, port = raw[name], SYMS.port[name]
        at = 0
        while at < len(b):
            n = 4 if len(b) - at >= 4 else 2 if len(b) - at >= 2 else 1
            v = int.from_bytes(b[at:at + n], "little")
            if n == 4 and 0x0C000000 <= v < 0x0E000000:
                v = translate(v, None, None)
                if v is None:
                    unknown += 1
                    at += n
                    continue
            out.append("poke 0x%x %d 0x%x" % (port + at, n, v))
            at += n
    if unknown:
        out.append("# %d pointers not known to the port" % unknown)
    return out


def select(team1, team2, keeps=()):
    """Two coins, both players in, each side's team poked in at the select, and on to the entrances
    (tools/hunt's menu.select)."""
    s = ["wait sys_work+0x14 4 64", "frames 60", "coin", "frames 20", "coin", "frames 20",
         "hold 4 start -", "frames 30", "hold 4 - start", "frames 20"]
    flags = [0, 0]
    for n in range(3):
        for side, team in ((0, team1), (1, team2)):
            w = (n * 2 + side) * PLW
            s += ["wait sel_w+0x%x 1 %d" % (0x34 + side, n + 1), "frames 10",
                  "poke pl_work+0x%x 1 %d" % (w + 0x52C, team[n]), "poke pl_work+0x%x 1 %d" % (w + 1, team[n])]
            k = ["lp", "-"] if side == 0 else ["-", "lp"]
            flags[side] |= 1 << n
            s += ["until sel_w+0x%x 1 0x%x %s %s" % (2 + side, flags[side], k[0], k[1])]
            flags[side] |= 16 << n
            s += ["until sel_w+0x%x 1 0x%x %s %s" % (2 + side, flags[side], k[0], k[1])]
    s += list(keeps) + ["until vs_sys_work+0 1 2 lp lp", "unkeep"]
    return s


def script(fight, lag, ns, ps, session=False):
    first = fight.frames[0][1]
    pl = first["pl_work"]
    if session:
        s = ["# the session from the select", "wait sys_work+0x14 4 64", "frames 60", "coin", "frames 20", "coin",
             "frames 20", "hold 4 start -", "frames 30", "hold 4 - start", "waitne sel_w+0xb 1 0",
             "# the work Flycast had as the select began"]
        s += pokes("sel_w", 0, SEL_FIELDS, first["sel_w"], ns, ps)
        s += ["poke stage_no 1 %d" % first["stage_no"][0]]
    else:
        team = [[pl[(n * 2 + side) * PLW + 1] for n in range(3)] for side in range(2)]
        keeps = ["keep stage_no 1 %d" % first["stage_no"][0]]
        keeps += ["keep pl_work+0x%x 1 %d" % (p * PLW + 0x52D, pl[p * PLW + 0x52D]) for p in range(6)]
        s = ["# fight %d: teams %s %s" % (fight.n, team[0], team[1])] + select(team[0], team[1], keeps)
        s += ["# the work Flycast had as the entrances began"]
    for p in range(6):
        s += pokes("pl_work", p * PLW, PL_FIELDS, pl[p * PLW:(p + 1) * PLW], ns, ps)
    s += pokes("vs_sys_work", 0, VS_FIELDS, first["vs_sys_work"], ns, ps)
    s += pokes("sys_work", 0, SYS_FIELDS, first["sys_work"], ns, ps)
    s += ["poke sw_work+0x%x 2 0x%x" % (o, struct.unpack_from("<H", first["sw_work"], o)[0]) for o in range(0, 0x28, 2)]
    s += globals_copy(first["globals"])
    s += ["poke nl_random_next 4 0x%x" % struct.unpack_from("<I", first["nl_random_next"])[0],
          "poke rnd_hi 4 0x%x" % struct.unpack_from("<I", first["rnd_hi"])[0],
          "poke rnd_hi+4 4 0x%x" % struct.unpack_from("<I", first["rnd_hi"], 4)[0], "trace on"]
    sw = [(struct.unpack_from("<H", d["sw_work"], 0)[0], struct.unpack_from("<H", d["sw_work"], 0x14)[0])
          for _, d in fight.frames]
    s.append("now %s %s" % (keys(sw[min(1, len(sw) - 1)][0]), keys(sw[min(1, len(sw) - 1)][1])))
    join = joins(fight) if session else {}
    for g in range(len(sw)):
        if g in join:      # the port waits out its own load, then takes Flycast's clock
            prev = fight.frames[g - 1][1]["sw_work"]     # key_repeat counts vblanks, which a load has more of
            s += ["until %s - -" % join[g], "poke sys_work+0x1c 4 0x%x" % timer0(fight.frames[g][1])]
            s += ["poke sw_work+0x%x 4 0x%x" % (o, struct.unpack_from("<I", prev, o)[0]) for o in (0x10, 0x24)]
            s += ["now %s %s" % tuple(keys(k) for k in sw[min(g + 1, len(sw) - 1)])]
        if 0 < g <= RNG_SETTLE and not session:
            s.append("poke nl_random_next 4 0x%x" % struct.unpack_from("<I", fight.frames[g][1]["nl_random_next"])[0])
        a, b = sw[min(len(sw) - 1, max(0, g + lag))]
        s.append("hold 1 %s %s" % (keys(a), keys(b)))
    s.append("quit")
    return "\n".join(s) + "\n"


def run_port(text, out, exe):
    os.makedirs(out, exist_ok=True)
    path = os.path.join(out, "script.txt")
    open(path, "w").write(text)
    syms = port_syms(exe)
    e = dict(os.environ)
    e.update(MVC2_HUNT=path, MVC2_HUNT_SYMS=",".join("%s=0x%X" % kv for kv in syms.items()),
             MVC2_HUNT_DUMP=",".join("%s:0x%X" % d for d in DUMP), MVC2_HUNT_TRACE=os.path.join(out, "trace.bin"),
             MVC2_FRAMES="400000", MVC2_CLOCK="946684800", SDL_VIDEODRIVER="offscreen",
             PATH="C:/msys64/mingw32/bin;" + e.get("PATH", ""))
    with open(os.path.join(out, "run.log"), "w") as log:
        r = subprocess.run([exe, "--arcade", os.path.join(DECOMP, "roms", "mvsc2")], cwd=out, env=e,
                           stdout=log, stderr=subprocess.STDOUT, timeout=3600)
    return r.returncode


def port_frames(path):
    size = 8 + sum(n for _, n in DUMP)
    data = open(path, "rb").read()
    out = []
    for i in range(0, len(data) - size + 1, size):
        rec, parts, at = data[i:i + size], {}, 8
        for name, n in DUMP:
            parts[name] = rec[at:at + n]
            at += n
        out.append(parts)
    return out


def show(f, raw):
    if f[3] == "f" and f[1] % 4 == 0:
        return " ".join("%.4f" % v for v in struct.unpack("<%df" % (f[1] // 4), raw))
    if f[1] in (1, 2, 4):
        return str(int.from_bytes(raw, "little", signed=True))
    return raw.hex()


def same(f, a, b):
    if a == b:
        return True
    if f[3] == "f" and f[1] % 4 == 0:
        va, vb = struct.unpack("<%df" % (f[1] // 4), a), struct.unpack("<%df" % (f[1] // 4), b)
        return all(abs(x - y) <= 1e-3 * max(1.0, abs(x)) or x != x for x, y in zip(va, vb))
    return False


def compare(fight, port, ns, ps, session=False):
    """The first game frame where the fight differs: [(what, port, flycast)], or None. Pointers are compared
    as the port's (translate)."""
    checks = [("vs_sys_work", "", f) for f in VS_FIELDS if not f[2].startswith("unk_")]
    checks += [("sys_work", "", f) for f in SYS_FIELDS if f[2] in ("game_timer", "play_timer", "game_speed")]
    for p in range(6):
        checks += [("pl_work", p, f) for f in PL_FIELDS if f[2] not in IGNORE and f[2] != "tlocal"]
    if session:
        checks += [("sel_w", "", f) for f in SEL_FIELDS if not f[2].startswith("unk_")]
    settle = 0 if session else RNG_SETTLE
    if session:     # by the clock, which each join sets; the port's frames in its own loads are left out
        fs, ps_ = segments([d for _, d in fight.frames]), segments(port)
        at = dict(((fs[g], timer0(d)), g) for g, (_, d) in enumerate(fight.frames))
        pairs = [(at[ps_[t], timer0(a)], a) for t, a in enumerate(port) if (ps_[t], timer0(a)) in at]
        # a load's last frames: Flycast may still be waiting on the ROM where the port is done
        near = set(g - k for g in joins(fight) for k in (1, 2))
        pairs = [(g, a) for g, a in pairs if g not in near]
    else:           # the port's trace starts a game frame after the sync
        pairs = [(t + 1, port[t]) for t in range(min(len(port), len(fight.frames) - 1))]
    for g, a in pairs:
        t, b = g - 1, fight.frames[g][1]
        if a["stage_no"][0] != b["stage_no"][0]:
            return t + 1, [("stage_no", str(a["stage_no"][0]), str(b["stage_no"][0]))]
        if a["nl_random_next"] != b["nl_random_next"] and t >= settle:
            return t + 1, [("nl_random_next", a["nl_random_next"].hex(), b["nl_random_next"].hex())]
        if a["sw_work"][:2] != b["sw_work"][:2] or a["sw_work"][0x14:0x16] != b["sw_work"][0x14:0x16]:
            return t + 1, [("sw_work.now", a["sw_work"][:2].hex() + " " + a["sw_work"][0x14:0x16].hex(),
                        b["sw_work"][:2].hex() + " " + b["sw_work"][0x14:0x16].hex())]
        diffs = []
        for name, p, f in checks:
            base = p * PLW if p != "" else 0
            if name == "pl_work" and a["pl_work"][base] == 0 and b["pl_work"][base] == 0:
                continue
            x, y = a[name][base + f[0]:base + f[0] + f[1]], b[name][base + f[0]:base + f[0] + f[1]]
            if f[3] == "p":
                if f[1] != 4:
                    continue
                t_y = translate(struct.unpack("<I", y)[0], ns, ps)
                pool = [ps["work_buff"] <= v < ps["work_buff"] + SIZES["work_buff"] for v in (t_y or 0, struct.unpack("<I", x)[0])]
                if t_y is not None and t_y != struct.unpack("<I", x)[0] and not all(pool):     # effect slots: not synced
                    diffs.append(("%s%s.%s" % (name, "[%d]" % p if p != "" else "", f[2]), "%08x" % struct.unpack("<I", x)[0],
                                  "%08x (port %08x)" % (struct.unpack("<I", y)[0], t_y)))
                continue
            if not same(f, x, y):
                diffs.append(("%s%s.%s" % (name, "[%d]" % p if p != "" else "", f[2]), show(f, x), show(f, y)))
        if diffs:
            return t + 1, diffs
    return None


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    opts = dict((a[2:].split("=", 1) + [""])[:2] for a in sys.argv[1:] if a.startswith("--"))
    scrd_dir, out = os.path.abspath(args[0]), os.path.abspath(args[1])
    exe = opts.get("exe") or os.path.join(BUILD, "mvc2.exe")
    lag = int(opts.get("lag", "2"))
    want = int(opts["fight"]) if "fight" in opts else None
    global SYMS
    SYMS = syms.Symbols(exe, os.path.join(BUILD, "arc.o"))
    ns = naomi_syms()
    session = "session" in opts
    if session:
        fights = read_session(scrd_dir, ns, int(opts["session"] or 400000))
    else:
        fights = read_fights(scrd_dir, ns, want)
    ps = port_syms(exe)
    if "write" in opts:
        head = ["sym %s 0x%X" % kv for kv in sorted(ps.items())]
        text = script(fights[0], lag, ns, ps, session)
        open(opts["write"], "w", newline="\n").write("\n".join(head) + "\n" + text)
        return

    def one(fight):
        fo = os.path.join(out, "session" if session else "fight_%d" % fight.n)
        rc = run_port(script(fight, lag, ns, ps, session), fo, exe)
        trace = os.path.join(fo, "trace.bin")
        port = port_frames(trace) if os.path.exists(trace) else []
        res = compare(fight, port, ns, ps, session)
        first = fight.frames[0][1]["head"]
        entry = {"fight": fight.n, "vblank": fight.start, "dojo_frame": first[0], "game_frames": len(fight.frames),
                 "port_frames": len(port), "exit": rc}
        lines = []
        if res is None:
            entry["result"] = "same" if len(port) >= len(fight.frames) - 1 else "port stopped early"
            lines.append("fight %d (dojo frame %d): %s for %d game frames" % (fight.n, first[0], entry["result"],
                                                                              len(port)))
            if entry["result"] == "same":
                os.remove(trace)
        else:
            t, diffs = res
            entry.update(result="differs", t=t, dojo_frame_at=fight.frames[t][1]["head"][0], diffs=diffs[:30])
            lines.append("fight %d (dojo frame %d): differs at game frame %d (dojo frame %d)" % (
                fight.n, first[0], t, entry["dojo_frame_at"]))
            lines += ["  %s: port %s flycast %s" % d for d in diffs[:30]]
        print(chr(10).join(lines), flush=True)
        return entry

    with ThreadPoolExecutor(int(opts.get("j", "4"))) as ex:
        results = list(ex.map(one, fights))
    json.dump(results, open(os.path.join(out, "results.json"), "w"), indent=1)


if __name__ == "__main__":
    main()
