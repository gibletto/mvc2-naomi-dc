"""Cut from your own ROM the few things the build does not make from source.

usage: mvc2rom.py ROM OUTDIR

ROM is epr-23085a.ic11 of the MAME set mvsc2: the file, the set's zip, or a folder with either.
Written to OUTDIR:

  data_340000.bin  seven model files that the game never reads, and the 0xFF after them
  hole_*.bin       three runs of the test program's file that hold left-overs of older builds
  nla6149.h        the program Sega's library sends to the I/O board's own processor
"""
import hashlib
import sys
import zipfile
from pathlib import Path

NAME = "epr-23085a.ic11"
SHA1 = "f58c31b245fc33fa541f9f074548402a63f7c3d3"
# file, offset in the ROM, length
PIECES = (("data_340000.bin", 0x340000, 0x80000), ("hole_3E6C00.bin", 0x3E6C00, 0x2000),
          ("hole_3E9400.bin", 0x3E9400, 0x1C00), ("hole_3F5C00.bin", 0x3F5C00, 0x400))
IO_PROGRAM = (0x246F70, 0x1FF0)


def read(path):
    if path.is_dir():
        found = [p for p in (path / NAME, path / "mvsc2.zip") if p.exists()]
        if not found:
            sys.exit(f"put {NAME} or mvsc2.zip in {path}")
        path = found[0]
    if not path.exists():
        sys.exit(f"{path} is not there")
    if path.suffix.lower() == ".zip":
        with zipfile.ZipFile(path) as z:
            if NAME not in z.namelist():
                sys.exit(f"{path} has no {NAME}")
            return z.read(NAME)
    return path.read_bytes()


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    rom, out = read(Path(sys.argv[1])), Path(sys.argv[2])
    if hashlib.sha1(rom).hexdigest() != SHA1:
        sys.exit(f"{NAME} is not the one this source builds (SHA-1 {SHA1})")
    out.mkdir(parents=True, exist_ok=True)
    for name, at, size in PIECES:
        (out / name).write_bytes(rom[at:at + size])
    at, size = IO_PROGRAM
    data = rom[at:at + size]
    lines = [", ".join(f"0x{b:02x}" for b in data[i:i + 16]) for i in range(0, size, 16)]
    (out / "nla6149.h").write_text(",\n".join(lines) + "\n")
    print(f"{NAME} is the arcade's; what the build takes from it is in {out}")


if __name__ == "__main__":
    main()
