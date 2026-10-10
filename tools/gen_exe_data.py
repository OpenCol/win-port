#!/usr/bin/env python3
"""What the port took from COLONIZE.EXE at startup, compiled in (roadmap W10).

    python3 tools/gen_exe_data.py [COLONIZE.EXE]            write src/gen/exe_data.c
    python3 tools/gen_exe_data.py [COLONIZE.EXE] --check    fail if it is not what this writes

COLONIZE.EXE defaults to $COL_GAME_DIR's; it must be the patched 1995 release. The file holds:

  - the globals' first values: the bytes of each global src/gen/globals_init.c lists, where any
    is not zero (a zero one needs nothing: the segment starts zeroed), and the bytes around
    the two places a relocated pointer sends to data no global models;
  - the relocations in those globals (far pointers the Win16 loader patched);
  - the segments' sizes and kinds, which the conversion needs;
  - the menus (128 the main menu, 129 the minimap's, 130 the Cheat menu) and dialog 400 (an
    empty template nothing opens), as the Win16 resource bytes, one menu item to a line.

src/host/w16_globals_init.c converts the globals from these exactly as it did from the file, and
src/host/w16_res.c answers FindResource/LoadMenu for COLONIZE.EXE's menus and dialog from them.
What is left in COLONIZE.EXE for the port is the 22 pictures (docs/replacement-art.md).
Daniel's decision (2026-10-10): these tables and menus go into the port's source; roadmap rule 8
keeps every other game file out.
"""
import hashlib
import os
import re
import struct
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "src", "gen", "exe_data.c")
PATCHED = "59a0e173452924603112cf410483021c02ef0cd3512e310d8966b21e0c176291"
LITERAL_WINDOW = 256           # bytes kept after a relocation's target that no global models


def globals_list():
    src = open(os.path.join(ROOT, "src", "gen", "globals_init.c")).read()
    out = []
    for m in re.finditer(r"\{ \(void \*\)&?\(?([^,]+?)\)?, (\d+), 0x([0-9a-f]+), (\d+), (\d+), [^,]+, (\d+) \},\s+/\* (\S+)", src):
        out.append((m.group(7), int(m.group(2)), int(m.group(3), 16), int(m.group(4)) * int(m.group(5))))
    return out


def image(exe):
    """{seg: (is_data, alloc, bytes)} and the relocations, as w16_globals_init.c read them."""
    r16 = lambda p: exe[p] | exe[p + 1] << 8
    ne = r16(0x3c)
    nsegs, segtab, shift = r16(ne + 0x1c), ne + r16(ne + 0x22), r16(ne + 0x32)
    segs, fixes = {}, []
    for i in range(nsegs):
        e = segtab + 8 * i
        off, ln, fl, mn = r16(e), r16(e + 2), r16(e + 4), r16(e + 6)
        n = ln or (0x10000 if off else 0)
        alloc = mn or 0x10000
        data = bytes(exe[(off << shift):(off << shift) + n]) if off else b""
        segs[i + 1] = (fl & 1, max(alloc, n) if fl & 1 else 0, data)
        if not (fl & 1 and off and fl & 0x100):
            continue
        rel = (off << shift) + n
        for k in range(r16(rel)):
            rr = exe[rel + 2 + 8 * k:rel + 10 + 8 * k]
            if rr[0] == 3 and rr[1] & 3 == 0 and rr[4] != 0xff:       # far address, internal, fixed seg
                s = rr[2] | rr[3] << 8
                while s != 0xffff and s + 4 <= n:
                    nxt = data[s] | data[s + 1] << 8
                    fixes.append((i + 1, s, rr[4], rr[6] | rr[7] << 8))
                    if rr[1] & 4:
                        break
                    s = nxt
    return segs, fixes


def resources(exe):
    r16 = lambda p: exe[p] | exe[p + 1] << 8
    ne = r16(0x3c)
    rt = ne + r16(ne + 0x24)
    shift, p, out = r16(rt), rt + 2, {}
    while r16(p):
        t, n = r16(p), r16(p + 2)
        p += 8
        for _ in range(n):
            off, ln, rid = r16(p), r16(p + 2), r16(p + 6)
            p += 12
            if t & 0x8000 and rid & 0x8000:
                out[(t & 0x7fff, rid & 0x7fff)] = bytes(exe[off << shift:(off << shift) + (ln << shift)])
    return out


def c_bytes(b, indent="    "):
    return "\n".join(indent + ", ".join("0x%02x" % x for x in b[i:i + 16]) + "," for i in range(0, len(b), 16))


def menu_lines(d):
    """the Win16 menu template, one item to a line: (bytes, comment)"""
    out = [(d[:4], "header: version 0, no extra")]
    p, depth = 4, 1
    while depth and p < len(d):
        flags = d[p] | d[p + 1] << 8
        q = p + 2
        if not flags & 0x10:                                 # MF_POPUP has no id
            mid = d[q] | d[q + 1] << 8
            q += 2
        e = d.index(b"\0", q)
        text = d[q:e].decode("latin1").replace("\t", "\\t")
        out.append((d[p:e + 1], ("  " * (depth - 1)) + ("popup %r" % text if flags & 0x10 else
                                 "separator" if not text and not mid else "%r id %d" % (text, mid)) +
                    (" (end)" if flags & 0x80 else "")))
        p = e + 1
        if flags & 0x10:
            depth += 1
        if flags & 0x80:
            depth -= 1
            # an MF_END on the last item of a popup closes it; the popup item's own END closes its level
    return out, p


def generate(exe_path):
    exe = open(exe_path, "rb").read()
    sha = hashlib.sha256(exe).hexdigest()
    if sha != PATCHED:
        sys.exit("%s is not the patched 1995 COLONIZE.EXE (SHA-256 %s)" % (exe_path, sha))
    segs, fixes = image(exe)
    G = globals_list()

    def owner(seg, off):
        return next((g for g in G if g[1] == seg and g[2] <= off < g[2] + g[3]), None)

    runs = []                                    # (seg, off, bytes, comment)
    for name, seg, off, span in sorted(G, key=lambda g: (g[1], g[2])):
        b = segs[seg][2][off:off + span] if segs[seg][0] else b""
        if any(b):
            runs.append((seg, off, b, name))
    used = [f for f in fixes if owner(f[0], f[1])]
    for seg, off, tseg, toff in used:
        if segs[tseg][0] and not owner(tseg, toff):  # a pointer into data no global models
            b = segs[tseg][2][toff:toff + LITERAL_WINDOW]
            lo = next((i for i, x in enumerate(b) if x), None)
            if lo is not None:
                hi = len(b) - next(i for i, x in enumerate(reversed(b)) if x)
                runs.append((tseg, toff + lo, b[lo:hi], "what %s points to (%d:%#06x, no global)" % (owner(seg, off)[0], tseg, toff)))
    # a window may overlap a global: keep the global's bytes, which are the same file bytes
    runs.sort(key=lambda r: (r[0], r[1]))
    res = resources(exe)

    L = ["/* GENERATED by tools/gen_exe_data.py from the patched 1995 COLONIZE.EXE (SHA-256 %s...):" % PATCHED[:16],
         " * what the port used to read from that file at startup, compiled in -- the globals' first",
         " * values (src/host/w16_globals_init.c converts them) and the menus and dialog",
         " * (src/host/w16_res.c). The 22 pictures still come from the file (docs/replacement-art.md).",
         " * Edit the menus here (one item to a line); regenerate everything else.",
         " */",
         "#include <stddef.h>",
         "",
         "struct PortExeSeg { unsigned char is_data; unsigned length; };",
         "struct PortExeBytes { unsigned short seg, off, len; const unsigned char *bytes; };",
         "struct PortExeFix { unsigned short seg, off, tseg, toff; };",
         "struct PortExeResource { unsigned short type, id; unsigned length; const unsigned char *bytes; };",
         "",
         "/* the segments: a data segment's size as loaded (its bytes start zeroed), 0 for code */",
         "const unsigned port_exe_seg_count = %d;" % len(segs),
         "const struct PortExeSeg port_exe_segs[%d] = {" % (len(segs) + 1),
         "    { 0, 0 },"]
    for i in range(1, len(segs) + 1):
        L.append("    { %d, %#x },   /* %d */" % (segs[i][0], segs[i][1], i))
    L += ["};", ""]
    for k, (seg, off, b, name) in enumerate(runs):
        L += ["/* %s: %d:%#06x, %d bytes */" % (name, seg, off, len(b)),
              "static const unsigned char b%d[%d] = {" % (k, len(b)), c_bytes(b), "};"]
    L += ["", "const unsigned port_exe_byte_count = %d;" % len(runs),
          "const struct PortExeBytes port_exe_bytes[%d] = {" % len(runs)]
    for k, (seg, off, b, name) in enumerate(runs):
        L.append("    { %d, %#06x, %d, b%d },   /* %s */" % (seg, off, len(b), k, name))
    L += ["};", "", "/* far pointers the Win16 loader patched inside those globals: slot -> target */",
          "const unsigned port_exe_fix_count = %d;" % len(used),
          "const struct PortExeFix port_exe_fixes[%d] = {" % len(used)]
    for seg, off, tseg, toff in used:
        t = owner(tseg, toff)
        L.append("    { %d, %#06x, %d, %#06x },   /* %s -> %s */" % (seg, off, tseg, toff, owner(seg, off)[0],
                                                               t[0] if t else "data no global models"))
    L += ["};", ""]
    entries = []
    for (t, rid) in sorted(k for k in res if k[0] in (4, 5)):
        d = res[(t, rid)]
        name = "menu%d" % rid if t == 4 else "dialog%d" % rid
        if t == 4:
            lines, end = menu_lines(d)
            L.append("/* MENU %d */" % rid)
            L.append("static const unsigned char %s[] = {" % name)
            for b, c in lines:
                L.append("    %s   /* %s */" % (" ".join("0x%02x," % x for x in b), c.replace("*/", "* /")))
            L.append("};")
        else:
            d = d.rstrip(b"\0")
            d = d + b"\0" * 4                      # the template's trailing class/caption/font ends
            L.append("/* DIALOG %d: an empty template (no controls, no caption, MS Sans Serif 8); nothing opens it */" % rid)
            L += ["static const unsigned char %s[] = {" % name, c_bytes(d), "};"]
        entries.append((t, rid, name))
    L += ["", "const unsigned port_exe_resource_count = %d;" % len(entries),
          "const struct PortExeResource port_exe_resources[%d] = {" % len(entries)]
    for t, rid, name in entries:
        L.append("    { %d, %d, sizeof %s, %s }," % (t, rid, name, name))
    L += ["};", ""]
    return "\n".join(L)


def main(argv):
    args = [a for a in argv if not a.startswith("--")]
    exe = args[0] if args else os.path.join(os.environ.get("COL_GAME_DIR", ""), "COLONIZE.EXE")
    text = generate(exe)
    if "--check" in argv:
        cur = open(OUT).read() if os.path.exists(OUT) else ""
        if cur != text:
            sys.exit("src/gen/exe_data.c: STALE (python3 tools/gen_exe_data.py)")
        print("src/gen/exe_data.c: current")
        return
    with open(OUT, "w") as f:
        f.write(text)
    print("src/gen/exe_data.c: written")


if __name__ == "__main__":
    main(sys.argv[1:])
