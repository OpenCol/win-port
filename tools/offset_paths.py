#!/usr/bin/env python3
"""A game byte offset into a class, as a member path the compiler evaluates (roadmap W9).

    python3 tools/offset_paths.py OBJECT CLASS OFFSET...    (in the Linux machine)

The corpus reaches some members by the game's byte offset -- `(char *)w + 0x7ee` for a
CDControlWin's button. On a 64-bit target those move. This reads CLASS's layout from OBJECT's
i386 debug information (build/obj-winegcc/..., the game's layout) and prints, for each offset,
the path as tools/alias_paths.py writes it: __builtin_offsetof(...) + sizeof(base) ...
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from elftools.elf.elffile import ELFFile          # noqa: E402

import importlib.util                             # noqa: E402
spec = importlib.util.spec_from_file_location("ap", os.path.join(os.path.dirname(os.path.abspath(__file__)), "alias_paths.py"))
ap = importlib.util.module_from_spec(spec)
ap.main = lambda: None
spec.loader.exec_module(ap)


def main(argv):
    obj, cls = argv[0], argv[1]
    with open(obj, "rb") as f:
        dw = ELFFile(f).get_dwarf_info()
        t = None
        for cu in dw.iter_CUs():
            for die in cu.iter_DIEs():
                if die.tag in ("DW_TAG_class_type", "DW_TAG_structure_type") and ap.name_of(die) == cls and ap.size_of(die):
                    t = die
                    break
            if t:
                break
        if t is None:
            sys.exit("no %s with a size in %s" % (cls, obj))
        for off in argv[2:]:
            print("%s %s" % (off, ap.expression(cls, ap.find(t, int(off, 0)))))


main(sys.argv[1:])
