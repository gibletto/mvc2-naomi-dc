"""Cut from your own ROM the few things the build does not make from source.

usage: mvc2rom.py [--us] ROM OUTDIR

ROM is epr-23085a.ic11 of the MAME set mvsc2 (with --us, epr-23062a.ic22 of mvsc2u, the US set):
the file, the set's zip, or a folder with either. Written to OUTDIR:

  data_340000.bin  seven model files that the game never reads, and the 0xFF after them (on the US
                   set, six model files that it reads, three packed and three coded for the M2
                   board's security chip)
  hole_*.bin       three runs of the test program's file that hold left-overs of older builds
  nla6149.h        the program Sega's library sends to the I/O board's own processor
"""
import hashlib
import sys
import zipfile
from pathlib import Path

# the ROM's name, its set's zip, its SHA-1, the pieces (file, offset in the ROM, length) and the
# I/O board's program (offset, length)
EXPORT = ("epr-23085a.ic11", "mvsc2.zip", "f58c31b245fc33fa541f9f074548402a63f7c3d3", "the arcade's",
          (("data_340000.bin", 0x340000, 0x80000), ("hole_3E6C00.bin", 0x3E6C00, 0x2000),
           ("hole_3E9400.bin", 0x3E9400, 0x1C00), ("hole_3F5C00.bin", 0x3F5C00, 0x400)), (0x246F70, 0x1FF0))
US = ("epr-23062a.ic22", "mvsc2u.zip", "877ba02c92082567280afcb1ae40b3bbfc8a63e8", "the US arcade's",
      (("data_340000.bin", 0x340000, 0x80000), ("hole_3E6C00.bin", 0x3E6C00, 0x1C00),
       ("hole_3E9000.bin", 0x3E9000, 0x1C00), ("hole_3F5400.bin", 0x3F5400, 0x800)), (0x2470D0, 0x1FF0))


def read(path, name, zipname):
    if path.is_dir():
        found = [p for p in (path / name, path / zipname) if p.exists()]
        if not found:
            sys.exit(f"put {name} or {zipname} in {path}")
        path = found[0]
    if not path.exists():
        sys.exit(f"{path} is not there")
    if path.suffix.lower() == ".zip":
        with zipfile.ZipFile(path) as z:
            if name not in z.namelist():
                sys.exit(f"{path} has no {name}")
            return z.read(name)
    return path.read_bytes()


def main():
    args = sys.argv[1:]
    which = US if args[:1] == ["--us"] else EXPORT
    args = args[1:] if which is US else args
    if len(args) != 2:
        sys.exit(__doc__)
    name, zipname, sha1, whose, pieces, (at, size) = which
    rom, out = read(Path(args[0]), name, zipname), Path(args[1])
    if hashlib.sha1(rom).hexdigest() != sha1:
        sys.exit(f"{name} is not the one this source builds (SHA-1 {sha1})")
    out.mkdir(parents=True, exist_ok=True)
    for piece, start, length in pieces:
        (out / piece).write_bytes(rom[start:start + length])
    data = rom[at:at + size]
    lines = [", ".join(f"0x{b:02x}" for b in data[i:i + 16]) for i in range(0, size, 16)]
    (out / "nla6149.h").write_text(",\n".join(lines) + "\n")
    print(f"{name} is {whose}; what the build takes from it is in {out}")


if __name__ == "__main__":
    main()
