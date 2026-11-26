"""check_3dsx.py FILE.3dsx  The 3DS: fails when a 3dsx has a relocation the Homebrew Launcher's loader refuses, which
stops the whole file loading: an absolute one whose word, less the base, has a top nibble other than 0 (an address
below the base), or a relative one with a top nibble over 1. Run after 3dsxtool (port/cmake/3ds-native.cmake)."""
import struct
import sys


def page(n):
    return (n + 0xFFF) & ~0xFFF


def check(path):
    b = open(path, "rb").read()
    magic, head, rel_head, _, _, code, rodata, data, bss = struct.unpack_from("<4sHHIIIIII", b, 0)
    if magic != b"3DSX":
        return ["not a 3dsx"]
    sizes = [page(code), page(rodata), page(data)]
    starts = [0, sizes[0], sizes[0] + sizes[1]]
    counts = [struct.unpack_from("<%dI" % (rel_head // 4), b, head + i * rel_head) for i in range(3)]
    at = head + 3 * rel_head
    mem = bytearray(sum(sizes))
    for start, n in zip(starts, (code, rodata, data - bss)):
        mem[start:start + n] = b[at:at + n]
        at += n
    bad = []
    for seg in range(3):
        for kind in range(2):      # the absolute ones, then the relative ones; any further counts are skipped
            pos, end = starts[seg] // 4, (starts[seg] + sizes[seg]) // 4
            for _ in range(counts[seg][kind]):
                skip, patch = struct.unpack_from("<HH", b, at)
                at += 4
                pos += skip
                for _ in range(patch):
                    if pos >= end:
                        break
                    word = struct.unpack_from("<I", mem, pos * 4)[0]
                    if word >> 28 > kind:
                        bad.append("%s relocation at %x: %08x" % (("absolute", "relative")[kind], 0x100000 + pos * 4, word))
                    pos += 1
        at += 4 * sum(counts[seg][2:])
    return bad


if __name__ == "__main__":
    found = check(sys.argv[1])
    for line in found:
        print("check_3dsx: %s: %s" % (sys.argv[1], line))
    sys.exit(1 if found else 0)
