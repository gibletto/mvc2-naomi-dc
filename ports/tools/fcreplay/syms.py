"""The arcade program's symbols (the decomp's link map) and the same symbols in the port's executable, to
turn a pointer of the arcade's into the port's."""
import bisect, os, re, subprocess

DECOMP = os.environ.get("MVC2_DECOMP", os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))
NM = os.environ.get("FCR_NM", "C:/msys64/mingw32/bin/nm.exe")    # the executable's own binutils' nm
FILES = 0x0C400000      # from here main RAM holds the files the game loads, at the same addresses in both


def nm(path):
    out = []
    for line in subprocess.run([NM, path], capture_output=True, text=True).stdout.splitlines():
        p = line.split()
        if len(p) == 3:
            out.append((int(p[0], 16), p[1], p[2].lstrip("_")))
    return out


class Symbols:
    def __init__(self, exe, arc_o):
        self.arcade, self.code = [], set()
        for line in open(os.path.join(DECOMP, "build", "mvc2.map")):
            m = re.match(r"\s*_(\w+)\s+H'([0-9A-F]{8})\s+(DAT|ENT)", line)
            if m:
                self.arcade.append((int(m.group(2), 16), m.group(1)))
                if m.group(3) == "ENT":
                    self.code.add(m.group(1))
        self.arcade.sort()
        self.addrs = [a for a, _ in self.arcade]
        self.by_name = dict((n, a) for a, n in self.arcade)
        self.sizes = {}
        self.port = self._port(exe, arc_o)
        self.port_addrs = sorted(set(a for a, _, _ in nm(exe)))
        self.routines, module = [], None
        for line in open(os.path.join(DECOMP, "symbols.txt")):
            p = line.split()
            if len(p) >= 2:
                module = p[2] if len(p) >= 3 else module
                self.routines.append((int(p[0], 16), p[1], module))
        self.routines.sort()
        self.r_addrs = [a for a, _, _ in self.routines]
        self.build, self.arc_o, self.exe = os.path.dirname(arc_o), arc_o, exe
        self.modules = None
        with open(exe, "rb") as f:
            head = f.read(20)
        self.thumb = int(head[:4] == b"\x7fELF" and head[18] == 40)    # an ARM build's code is Thumb: its pointers odd

    @staticmethod
    def _port(exe, arc_o):
        """Each name's address in the executable: the arcade program's copy, found by the shift of each
        section of arc.o that most of its names agree on; library names have one copy."""
        ex = {}
        for a, _, n in nm(exe):
            ex.setdefault(n, []).append(a)
        arc = nm(arc_o)
        out = {}
        for sect in set(t for _, t, _ in arc if t.lower() in "tbdr"):
            names = [(a, n) for a, t, n in arc if t == sect and n in ex]
            votes = {}
            for a, n in names:
                for e in ex[n]:
                    votes[e - a] = votes.get(e - a, 0) + 1
            if not votes:
                continue
            shift = max(votes, key=votes.get)
            for a, n in names:
                if a + shift in ex[n]:
                    out[n] = a + shift
        for n, adrs in ex.items():
            if n not in out and len(adrs) == 1:
                out[n] = adrs[0]
        return out

    def size(self, name):
        """The bytes a name holds: to the next name in the arcade program, or in the port, whichever is less."""
        if name not in self.sizes:
            a = self.by_name[name]
            i = bisect.bisect_right(self.addrs, a)
            n1 = self.addrs[i] - a if i < len(self.addrs) else 1 << 20
            p = self.port[name]
            j = bisect.bisect_right(self.port_addrs, p)
            self.sizes[name] = min(n1, self.port_addrs[j] - p if j < len(self.port_addrs) else n1)
        return self.sizes[name]

    def where(self, ptr):
        """The arcade symbol a pointer falls in, and how far in."""
        i = bisect.bisect_right(self.addrs, ptr) - 1
        if i < 0:
            return None, 0
        return self.arcade[i][1], ptr - self.arcade[i][0]

    def _module_routines(self):
        """Each module's routines in the port, from its own object and where gcc -r put it in arc.o."""
        objs = {}
        for path in open(os.path.join(self.build, "gen", "arc", "objects.rsp")).read().split():
            objs[os.path.basename(path).split(".")[0]] = path
        arc = [(a, n) for a, t, n in nm(self.arc_o) if t == "t"]
        count = {}
        for _, n in arc:
            count[n] = count.get(n, 0) + 1
        where = dict((n, a) for a, n in arc if count[n] == 1)
        shift = self.port["pl_work_move"] - where["pl_work_move"]
        out = {}
        for mod, path in objs.items():
            mine = [(a, n) for a, t, n in nm(path) if t.lower() == "t"]
            base = next((where[n] - a for a, n in mine if n in where), None)
            if base is not None:
                out[mod] = dict((n, a + base + shift) for a, n in mine)
        return out

    def routine(self, ptr):
        """A pointer into the arcade's code as the port's, by the routine it is in (symbols.txt) and its
        module; None if the port has no such routine. Only a routine's start is the same code in both."""
        i = bisect.bisect_right(self.r_addrs, ptr) - 1
        if i < 0 or self.routines[i][0] != ptr:
            return None
        _, name, mod = self.routines[i]
        if self.modules is None:
            self.modules = self._module_routines()
        if mod in self.modules and name in self.modules[mod]:
            return self.modules[mod][name]
        return None

    def translate(self, ptr):
        """An arcade pointer as the port's, or None when its symbol is unknown to the port."""
        if ptr >= FILES:
            return ptr
        name, off = self.where(ptr)
        if name in self.code:
            r = self.routine(ptr)
            return r | self.thumb if r is not None else None
        if name is None or name not in self.port or off >= self.size(name):
            return None     # a static of the module's, which the map leaves out
        return self.port[name] + off
