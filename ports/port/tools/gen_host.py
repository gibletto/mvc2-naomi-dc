"""Write the C a host build needs for what the game's objects (the first OBJDIR) use and no object defines.

usage: gen_host.py DECOMP NM OUTDIR OBJDIR... [--program=arcade|dc] [--prefix=NAME] [--underscore]

- the program's data, or a fixed RAM buffer: its address (host_defsyms.rsp). The program image is loaded
  there, and its words that point at the C's routines or data are relocated (pointers.tsv);
- a routine: a stub that says once that it was called.
The Dreamcast's addresses are masked to the RAM's. With --prefix the program's kept names take the prefix
(host_rename.txt), as the linker takes a local absolute name for 0. No ROM data is written.
"""
import bisect
import re
import subprocess
import sys
from pathlib import Path


class Program:
    """[start, end) ranges of its code and initialised data, and the decomp's files that describe it."""

    def __init__(self, name, symbols, pointers, sub, code, data, start, end):
        self.name, self.symbols, self.pointers, self.sub, self.code, self.data, self.start, self.end = (
            name, symbols, pointers, sub, code, data, start, end)

    def is_code(self, a):
        return any(lo <= a < hi for lo, hi in self.code)

    def is_data(self, a):
        return any(lo <= a < hi for lo, hi in self.data)


PROGRAMS = {
    # P and PSG; C, CSG, D and DSG; B, BX and BSG and the stack
    "arcade": Program("arcade", "symbols.txt", "pointers.tsv", "mvc2.sub",
                      [(0x0C000000, 0x0C227460)], [(0x0C227460, 0x0C269AF4)],
                      0x0C000000, 0x0C3FC000),
    # strt1's head, P, C and D; PSG, CSG and DSG; the stack, B, B$32 and BSG
    "dc": Program("Dreamcast", "symbols_dc.txt", "pointers_dc.tsv", "mvc2dc.sub",
                  [(0x8C010080, 0x8C132790), (0x8C1742C0, 0x8C1C3F24)],
                  [(0x8C010000, 0x8C010080), (0x8C132790, 0x8C1742C0), (0x8C1C3F24, 0x8C1C9D99)],
                  0x8C010000, 0x8C3539B0),
}
# spr_tbl.src's, in the code section
DATA_IN_CODE = {"spr_cnt_tbl", "spr_set_cnt_tbl"}
RAM_MASK = 0x1FFFFFFF      # any of the SH-4's mirrors of main RAM
# the host's C library has these
LIBC = {"memset", "memcpy", "memmove", "memcmp", "strcpy", "strncpy", "strlen", "strcmp", "strncmp", "strcat",
        "sprintf", "vsprintf", "printf", "sqrtf", "sinf", "cosf", "fabsf", "sqrt", "sin", "cos", "atan2f",
        "rand", "srand", "abs", "atoi", "strchr", "memchr", "floorf", "powf"}


def read_symbols(path):
    """({global name: address}, every address a symbol starts at, statics too)."""
    out, starts = {}, set()
    for line in open(path):
        p = line.split()
        if len(p) >= 2 and re.fullmatch(r"[0-9A-Fa-f]{8}", p[0]):
            starts.add(int(p[0], 16))
            if len(p) == 2:  # a third column names the module of a static
                out.setdefault(p[1], int(p[0], 16))
    return out, starts


def read_defines(path):
    """{name: address} of the link's define _name(ADDRESS)."""
    return {m[1]: int(m[2], 16) for m in re.finditer(r"^define _(\w+)\(([0-9A-Fa-f]+)\)", open(path).read(), re.M)}


def address_of(name, syms):
    if name in syms:
        return syms[name]
    m = re.fullmatch(r"(?:unk|func|loc)_([0-9A-F]{8})", name)
    return int(m[1], 16) if m else None


def run_nm(nm, dirs, out):
    """nm -S, the objects in a response file: their paths can pass a command line's length"""
    objs = [p.as_posix() for d in dirs for p in Path(d).rglob("*") if p.suffix in (".o", ".obj")]
    rsp = out / "nm_objects.rsp"
    rsp.write_text("".join('"%s"\n' % o.replace("\\", "\\\\").replace('"', '\\"') for o in objs), newline="\n")
    return subprocess.run([nm, "-S", "@" + str(rsp)], capture_output=True, text=True, check=True).stdout.splitlines()


def read_nm(lines, underscore):
    """({defined name: size or None}, undefined names), without the i686 leading underscore."""
    defined, undefined = {}, set()
    for line in lines:
        p = line.split()
        if len(p) < 2:
            continue
        kind, name = p[-2], p[-1]
        if underscore and name.startswith("_"):
            name = name[1:]
        if kind == "U":
            undefined.add(name)
        elif not kind.isupper():  # a static
            continue
        elif kind != "C" or name not in defined:  # a common's size is its length
            size = int(p[1], 16) if len(p) == 4 else int(p[0], 16) if kind == "C" and len(p) == 3 else None
            defined[name] = size
    return defined, undefined - set(defined)


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    decomp, nm, out, dirs = Path(args[0]), args[1], Path(args[2]), args[3:]
    opts = dict(a[2:].split("=", 1) for a in sys.argv[1:] if a.startswith("--") and "=" in a)
    prog = PROGRAMS[opts.get("program", "arcade")]
    out.mkdir(parents=True, exist_ok=True)
    syms, starts = read_symbols(decomp / prog.symbols)
    defines = read_defines(decomp / prog.sub)
    underscore = "--underscore" in sys.argv
    defined, missing = read_nm(run_nm(nm, dirs, out), underscore)
    _, game_missing = read_nm(run_nm(nm, dirs[:1], out), underscore)   # what the game itself uses (the first OBJDIR)
    missing &= game_missing

    # the host's symbols, by the program's address
    data, funcs, fixed = {}, [], {}
    for n in sorted(missing):
        a = defines.get(n, address_of(n, syms))
        if n.startswith("host_") or n.startswith("__") or n in LIBC:   # __: the compiler's helpers (__aeabi_idiv)
            continue
        if n in DATA_IN_CODE:
            data[n] = a
        elif a is None or n.startswith("func_") or prog.is_code(a) and not n.startswith("unk_"):  # unk_ is data
            funcs.append(n)
        elif not prog.start <= a < prog.end:
            fixed[n] = a
        else:
            data[n] = a
    # the words of the program's data that point at what the host defines; those into data left in place stay
    owners = sorted((a, n) for n, a in syms.items())
    owners += [(a, None) for a in starts if a not in syms.values()]   # statics: no name to resolve
    owners.sort(key=lambda x: (x[0], x[1] is None))
    okeys = [a for a, _ in owners]

    def target(v):
        """(host symbol, offset) for the program's address v, or None when it stays."""
        if not (prog.is_code(v) or prog.is_data(v)):
            return None
        i = bisect.bisect_right(okeys, v) - 1
        if i < 0:
            return None
        a, n = owners[i]
        if n is None:
            return "static", 0
        if n in defined or n in funcs:
            return n, v - a
        if prog.is_code(v):
            return "static", 0
        return None

    # masked to the RAM's address all the same
    relocs, extern, statics, masked = [], set(), 0, 0
    pointers = decomp / prog.pointers
    for line in open(pointers if pointers.is_file() else decomp / "build" / prog.pointers):
        p = line.split()
        if len(p) != 2:
            continue
        at, v = int(p[0], 16), int(p[1], 16)
        if not prog.is_data(at):
            continue
        t = target(v)
        if t is None or t[0] == "static":
            statics += t is not None
            if v & RAM_MASK != v:
                relocs.append((at & RAM_MASK, None, v & RAM_MASK))
                masked += 1
            continue
        relocs.append((at & RAM_MASK, t[0], t[1]))
        extern.add(t[0])

    c = [f"/* generated by port/tools/gen_host.py: the address words in the {prog.name} program's data that point at",
         "   what the host defines itself */", '#include "naomi_host.h"', ""]
    for n in sorted(extern):
        c.append(f"extern char {n}[];")
    c += ["", "const HOST_RELOC host_reloc[] = {"]
    for at, t, toff in sorted(relocs, key=lambda r: r[0]):
        if t is None:
            c.append(f"    {{ (char *){at:#010x}, (char *){toff:#010x} }},")
        else:
            c.append(f"    {{ (char *){at:#010x}, {t} + {toff:#x} }},")
    c += ["    { 0, 0 }", "};", ""]
    (out / "host_data.c").write_text("\n".join(c), newline="\n")

    s = ["/* generated by port/tools/gen_host.py: library routines the port does not implement yet */",
         '#include "naomi_host.h"', ""]
    for n in funcs:
        s.append(f"int {n}( void ) {{ host_stub( \"{n}\" ) ; return 0 ; }}")
    (out / "host_stubs.c").write_text("\n".join(s) + "\n", newline="\n")

    pre = "_" if "--underscore" in sys.argv else ""
    prefix = opts["prefix"] + "." if "prefix" in opts else ""
    places = sorted({**data, **fixed}.items())
    d = [f"-Wl,--defsym,{pre}{prefix}{n}={a & RAM_MASK:#x}" for n, a in places]
    (out / "host_defsyms.rsp").write_text("\n".join(d) + "\n", newline="\n")
    if prefix:
        (out / "host_rename.txt").write_text("".join(f"{pre}{n} {pre}{prefix}{n}\n" for n, _ in places), newline="\n")
    print(f"gen_host: {len(data)} data symbols in place, {len(relocs) - masked} relocations ({statics} to statics "
          f"left), {len(fixed)} fixed buffers, {len(funcs)} stubbed routines"
          + (f", {masked} words masked to the RAM" if masked else ""))


if __name__ == "__main__":
    main()
