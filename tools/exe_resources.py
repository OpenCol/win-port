#!/usr/bin/env python3
"""The pictures the port still takes from COLONIZE.EXE: extract them from your own copy.

    python3 tools/exe_resources.py /path/to/COLONIZE.EXE [OUT]      (OUT: ./exe-resources)

The port reads COLONIZE.EXE's own resources at run time (src/host/w16_res.c); 22 of them are
pictures: 12 cursors, 4 icons and 6 bitmaps. To run without COLONIZE.EXE they need
replacements (docs/replacement-art.md lists each one and what it is for). This writes each as
the Windows file a replacement would be (.cur, .ico, .bmp), as a PNG (transparent where the
original is), and sheet.png, all of them at 4x on one page.

It reads the 16-bit NE resource table itself and needs nothing beyond Python 3. Nothing it
writes goes into git (roadmap rule 8): run it on your own copy.
"""
import os
import struct
import sys
import zlib

RT_CURSOR, RT_BITMAP, RT_ICON, RT_GROUP_CURSOR, RT_GROUP_ICON = 1, 2, 3, 12, 14

# what each one is for (tools/../docs/replacement-art.md has the longer story)
ROLE = {
    ("cursor", 128): "map: dragging the selected unit away (goto)",
    ("cursor", 129): "map: move north", ("cursor", 130): "map: move north-east",
    ("cursor", 131): "map: move east", ("cursor", 132): "map: move south-east",
    ("cursor", 133): "map: move south", ("cursor", 134): "map: move south-west",
    ("cursor", 135): "map: move west", ("cursor", 136): "map: move north-west",
    ("cursor", 137): "a large arrow pointer (no use found in the code)", ("cursor", 138): "minimap window: a crosshair",
    ("cursor", 139): "an hourglass (no use found in the code)",
    ("icon", 128): "the game, main and map windows: a flag in a frame", ("icon", 129): "status window: a man in a frame",
    ("icon", 130): "Colonopedia window: a red book in a frame", ("icon", 131): "reports and minimap windows: a ship in a frame",
    ("bitmap", 301): "radio button, filled grey", ("bitmap", 302): "radio button with a black dot",
    ("bitmap", 303): "radio button, an empty ring", ("bitmap", 304): "check box with a tick",
    ("bitmap", 305): "check box, filled dark grey", ("bitmap", 306): "check box, filled light grey",
}


def resources(exe):
    ne = struct.unpack_from("<I", exe, 0x3c)[0]
    if exe[ne:ne + 2] != b"NE":
        sys.exit("not a 16-bit Windows program (no NE header)")
    rt = ne + struct.unpack_from("<H", exe, ne + 0x24)[0]
    shift = struct.unpack_from("<H", exe, rt)[0]
    p, out = rt + 2, {}
    while True:
        t = struct.unpack_from("<H", exe, p)[0]
        if not t:
            return out
        n = struct.unpack_from("<H", exe, p + 2)[0]
        p += 8
        for _ in range(n):
            off, ln, _fl, rid = struct.unpack_from("<HHHH", exe, p)
            p += 12
            if t & 0x8000 and rid & 0x8000:
                out[(t & 0x7fff, rid & 0x7fff)] = exe[off << shift:(off << shift) + (ln << shift)]


def dib(d):
    """(width, height, rgba rows top-down, bpp) of a DIB (BITMAPINFOHEADER, palette, bits)."""
    hs, w, h, _planes, bpp = struct.unpack_from("<IiiHH", d, 0)
    ncol = struct.unpack_from("<I", d, 32)[0] or (1 << bpp if bpp <= 8 else 0)
    pal = [(d[hs + 4 * i + 2], d[hs + 4 * i + 1], d[hs + 4 * i]) for i in range(ncol)]
    bits = hs + 4 * ncol
    stride = (w * bpp + 31) // 32 * 4
    rows = []
    for y in range(h - 1, -1, -1):
        r, row = bits + y * stride, []
        for x in range(w):
            if bpp == 24:
                b, g, rr = d[r + 3 * x:r + 3 * x + 3]
                row.append((rr, g, b, 255))
            else:
                v = d[r + x * bpp // 8] >> (8 - bpp - (x * bpp) % 8) & ((1 << bpp) - 1)
                row.append(pal[v] + (255,))
        rows.append(row)
    return w, h, rows, bpp, bits + stride * h


def masked(d):
    """an icon or cursor image: the XOR picture over the AND mask (height is doubled)."""
    hs, w, h2 = struct.unpack_from("<Iii", d, 0)
    h = h2 // 2
    bpp = struct.unpack_from("<H", d, 14)[0]
    ncol = struct.unpack_from("<I", d, 32)[0] or (1 << bpp if bpp <= 8 else 0)
    hdr = bytearray(d[:hs])
    struct.pack_into("<i", hdr, 8, h)
    w_, h_, rows, _, end = dib(bytes(hdr) + d[hs:])
    mstride = (w + 31) // 32 * 4
    out = []
    for y in range(h):
        r = end + (h - 1 - y) * mstride
        row = []
        for x in range(w):
            transparent = d[r + x // 8] >> (7 - x % 8) & 1
            px = rows[y][x]
            # AND 1 + XOR black: see-through; AND 1 + XOR colour: screen inverted (shown magenta)
            row.append((0, 0, 0, 0) if transparent and px[:3] == (0, 0, 0) else
                       (255, 0, 255, 255) if transparent else px)
        out.append(row)
    return w, h, out, bpp


def png(path, w, h, rows, scale=1):
    raw = b"".join(b"\0" + b"".join(bytes(px) * scale for px in row) * 1
                   for row in rows for _ in range(scale))
    def chunk(t, b):
        return struct.pack(">I", len(b)) + t + b + struct.pack(">I", zlib.crc32(t + b) & 0xffffffff)
    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w * scale, h * scale, 8, 6, 0, 0, 0))
                + chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b""))


def group(res, gtype, itype, cursor):
    """each group resource as an .ico/.cur file and its first image"""
    out = []
    for (t, gid), g in sorted(res.items()):
        if t != gtype:
            continue
        n = struct.unpack_from("<H", g, 4)[0]
        entries, images = [], []
        for i in range(n):
            e = g[6 + 14 * i:6 + 14 * i + 14]
            if cursor:
                w, h2, planes, bpp, size, nid = struct.unpack("<HHHHIH", e)
                img = res[(itype, nid)]
                hx, hy = struct.unpack_from("<HH", img, 0)
                img = img[4:]
                entries.append((w, h2 // 2, 0, hx, hy))
            else:
                w, h, colours, _r, planes, bpp, size, nid = struct.unpack("<BBBBHHIH", e)
                img = res[(itype, nid)]
                entries.append((w, h, colours, planes, bpp))
            hs, iw, ih2 = struct.unpack_from("<Iii", img, 0)
            ibpp = struct.unpack_from("<H", img, 14)[0]
            ncol = struct.unpack_from("<I", img, 32)[0] or (1 << ibpp if ibpp <= 8 else 0)
            ih = ih2 // 2
            size = hs + 4 * ncol + ((iw * ibpp + 31) // 32 * 4) * ih + ((iw + 31) // 32 * 4) * ih
            images.append(img[:size])
        f = struct.pack("<HHH", 0, 2 if cursor else 1, n)
        off = 6 + 16 * n
        for (a, b, c, d1, d2), img in zip(entries, images):
            f += struct.pack("<BBBBHHII", a & 0xff, b & 0xff, c, 0, d1, d2, len(img), off)
            off += len(img)
        out.append((gid, f + b"".join(images), images[0], entries[0]))
    return out


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        sys.exit(__doc__)
    exe = open(argv[0], "rb").read()
    out = argv[1] if len(argv) > 1 else "exe-resources"
    os.makedirs(out, exist_ok=True)
    res = resources(exe)
    sheet = []
    for kind, gtype, itype, ext in (("cursor", RT_GROUP_CURSOR, RT_CURSOR, "cur"), ("icon", RT_GROUP_ICON, RT_ICON, "ico")):
        for gid, filedata, img, entry in group(res, gtype, itype, kind == "cursor"):
            name = "%s-%d" % (kind, gid)
            with open(os.path.join(out, name + "." + ext), "wb") as f:
                f.write(filedata)
            w, h, rows, bpp = masked(img)
            png(os.path.join(out, name + ".png"), w, h, rows)
            extra = ", hotspot %d,%d" % entry[3:5] if kind == "cursor" else ""
            print("%-11s %2dx%-2d %2d colours%-16s %s" % (name, w, h, 1 << bpp, extra, ROLE.get((kind, gid), "")))
            sheet.append((name, w, h, rows))
    for (t, rid), d in sorted(res.items()):
        if t != RT_BITMAP:
            continue
        name = "bitmap-%d" % rid
        w, h, rows, bpp, end = dib(d)
        with open(os.path.join(out, name + ".bmp"), "wb") as f:
            f.write(b"BM" + struct.pack("<IHHI", 14 + end, 0, 0, 14 + struct.unpack_from("<I", d, 0)[0] +
                                         4 * (struct.unpack_from("<I", d, 32)[0] or (1 << bpp))) + d[:end])
        png(os.path.join(out, name + ".png"), w, h, rows)
        print("%-11s %2dx%-2d %2d colours%-16s %s" % (name, w, h, 1 << bpp, "", ROLE.get(("bitmap", rid), "")))
        sheet.append((name, w, h, rows))
    # sheet.png: every picture at 4x on a grey ground, six to a row
    cell, gap, per = 32 * 4, 12, 6
    nrow = (len(sheet) + per - 1) // per
    W, H = per * (cell + gap) + gap, nrow * (cell + gap) + gap
    page = [[(200, 200, 200, 255)] * W for _ in range(H)]
    for i, (_name, w, h, rows) in enumerate(sheet):
        ox, oy = gap + (i % per) * (cell + gap), gap + (i // per) * (cell + gap)
        for y in range(h * 4):
            for x in range(w * 4):
                px = rows[y // 4][x // 4]
                if px[3]:
                    page[oy + y][ox + x] = px
    png(os.path.join(out, "sheet.png"), W, H, page)
    print("%d pictures in %s (sheet.png: all of them at 4x, in this order, six to a row)" % (len(sheet), out))


if __name__ == "__main__":
    main(sys.argv[1:])
