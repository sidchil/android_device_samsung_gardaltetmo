#!/usr/bin/env python3
"""Rename libMali.so's pthread_create/pthread_join imports (same length) so
they resolve to libshim_mali's validating wrappers. Idempotent.

usage: patch-mali.py path/to/libMali.so
"""
import struct
import sys

RENAMES = {b"pthread_create": b"t399_th_create", b"pthread_join": b"t399_th_join"}
DT_STRTAB, DT_STRSZ = 5, 10


def main(path):
    d = bytearray(open(path, "rb").read())
    assert d[:4] == b"\x7fELF" and d[4] == 1 and d[5] == 1, "need ELF32 LE"
    phoff, = struct.unpack_from("<I", d, 28)
    phentsize, phnum = struct.unpack_from("<HH", d, 42)
    loads, dyn = [], None
    for i in range(phnum):
        p_type, p_off, p_vaddr, _, p_filesz = struct.unpack_from("<IIIII", d, phoff + i * phentsize)
        if p_type == 1:
            loads.append((p_vaddr, p_off, p_filesz))
        elif p_type == 2:
            dyn = (p_off, p_filesz)

    def v2o(va):
        for v, o, s in loads:
            if v <= va < v + s:
                return va - v + o
        raise ValueError(hex(va))

    tags = {}
    for off in range(dyn[0], dyn[0] + dyn[1], 8):
        tag, val = struct.unpack_from("<iI", d, off)
        if tag == 0:
            break
        tags[tag] = val
    start = v2o(tags[DT_STRTAB])
    end = start + tags[DT_STRSZ]
    changed = 0
    for old, new in RENAMES.items():
        assert len(old) == len(new)
        if d.find(b"\0" + new + b"\0", start, end) >= 0:
            print("already patched:", new.decode())
            continue
        i = d.find(b"\0" + old + b"\0", start, end)
        if i < 0:
            sys.exit("not found in .dynstr: " + old.decode())
        d[i + 1:i + 1 + len(old)] = new
        changed += 1
        print("renamed", old.decode(), "->", new.decode(), "at", hex(i + 1))
    if changed:
        open(path, "wb").write(d)


if __name__ == "__main__":
    main(sys.argv[1])
