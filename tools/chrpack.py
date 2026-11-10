"""Pack the characters' data for the ROM (0x250000): the build's step after data/*.c is compiled.

Each character has nine parts, each a file of data/ linked alone at 0 (DIR/NAME.bin). A part is
padded with zeros to 32 bytes; a table of 16 offsets goes before the nine, and the whole is packed
with Capcom's LZ (lz_unpack16 unpacks it) and padded to 32 bytes. A table of 64 offsets goes before
the packs, and 0xFF after them to the length the program reads, 0xF0000 bytes.
The packer is greedy, as Capcom's was: at each word the longest run of two words or more that
starts in the 0x7FF words before, the nearest of them if several are as long, else the word itself.
usage: chrpack.py DIR OUT
"""
import bisect
import struct
import sys
from pathlib import Path

PARTS = ("char_%02x", "frame_box%02x", "hit_%02x", "hit_dt%02x", "taiki%02x", "atck%02x", "wari%02x",
         "yok%02x", "lib%02x")
CHARACTERS = 0x3b
SIZE = 0xf0000


def pad(b, fill=b"\0"):
    return b + fill * (-len(b) % 32)


def pack(words):
    """The packed form of words: flag words, each before its 16 codes (the first code's bit is the
    top one). A clear bit is a word as it is; a set bit is a run: its length in the top five bits
    and how far back it starts in the low eleven, or, for 32 words and more, the distance alone
    and then the length. A run of no length from no distance ends it."""
    codes, at, n = [], 0, len(words)
    seen = {}  # a pair of words: where it has been, in order
    while at < n:
        best, back = 1, 0
        if at + 1 < n:
            was = seen.get((words[at], words[at + 1]), ())
            for q in reversed(was[bisect.bisect_left(was, at - 0x7ff):]):
                run = 2
                while at + run < n and words[q + run] == words[at + run]:
                    run += 1
                if run > best:
                    best, back = run, at - q
        best = min(best, 0xffff)
        codes.append((best, back) if back else None)
        for i in range(at, min(at + best, n - 1)):
            seen.setdefault((words[i], words[i + 1]), []).append(i)
        at += best
    codes.append((0, 0))
    out, at = [], 0
    for i in range(0, len(codes), 16):
        group = codes[i:i + 16]
        out.append(sum(0x8000 >> j for j, c in enumerate(group) if c))
        for c in group:
            if c is None:
                out.append(words[at])
                at += 1
            else:
                out += [c[0] << 11 | c[1]] if 0 < c[0] < 32 else [c[1], c[0]]
                at += c[0]
    return struct.pack("<%dH" % len(out), *out)


def main():
    folder, out = Path(sys.argv[1]), Path(sys.argv[2])
    table, packs = [], b""
    for no in range(CHARACTERS):
        parts = [pad((folder / (name % no + ".bin")).read_bytes()) for name in PARTS]
        offs, at = [], 0x40
        for p in parts:
            offs.append(at)
            at += len(p)
        whole = struct.pack("<16I", *offs, *[0] * 7) + b"".join(parts)
        table.append(0x100 + len(packs))
        packs += pad(pack(struct.unpack("<%dH" % (len(whole) // 2), whole)))
    data = struct.pack("<64I", *table, *[0] * (64 - CHARACTERS)) + packs
    if len(data) > SIZE:
        sys.exit(f"the packs are {len(data):#x} bytes: over {SIZE:#x}")
    out.write_bytes(data + b"\xff" * (SIZE - len(data)))


if __name__ == "__main__":
    main()
