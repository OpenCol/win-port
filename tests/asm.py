#!/usr/bin/env python3
"""The assembly differential: the game's assembly routines against the port's C (roadmap W5).

    python3 tests/asm.py              every routine on its argument sets
    python3 tests/asm.py --record     also write status/asm.json (tools/status.py reads it)
    python3 tests/asm.py --only NAME  one routine
    python3 tests/asm.py --assets     the game's own assets instead (also part of --record)

Segments 17-19 hold the game's own assembly: 12 entry points (src/asm/README.md has the
inventory; the rest of segment 19 is Borland's runtime, which the port's compiler supplies).
Each runs twice on the same bytes:
  - the ORIGINAL, in col-win-re's emulator, at its address in the shipped executable, each
    buffer in a block of its own, passed as a far pointer;
  - the PORT's (src/asm), built as the port builds it, in tests/asm/harness.c under Wine.
Compared: every buffer afterwards (each with 256 guard bytes either side, so a write outside
it shows), the return value (AX, or DX:AX for a long) and the random seed at DGROUP:0x3f00.

The inputs are random but well formed -- a decoder handed noise runs off its buffer on both
sides and proves nothing: FLIC chunks, GIF LZW streams (an encoder here) and sprites are
generated, every rectangle lies inside its bitmap, touching each edge in turn, every stride
sign and every scale of the sprite map. The seed is fixed, so a run is repeatable.

With --assets the inputs are the game's own, from $COL_GAME_DIR's NE files (types by their
RT_NAMETABLE names), each passed as the game's caller passes it:
  SPRT  every sprite, as Sprite_DrawSprite draws it (its scale map built as
        SetGlobalSpriteScale builds it, its clip as IntersectRect leaves it): 1:1 inside the
        canvas and through the mono blitter, then two of the game's scales in turn (SCALES),
        clipped at the top left of a bottom-up canvas and at the bottom right;
  CvPc  every canvas, as GRPort_LoadPicture decompresses it (MSPicDecompress's tables);
  FLIC  every chunk the assembly decodes (7, 15, 16) of every frame, as Flic_DecodeFrame
        passes it, over a random frame buffer (a decoder only writes: output equal over any
        background is the same frame).
Nothing from the assets is written anywhere; only counts are recorded.
"""
import json
import os
import pickle
import random
import struct
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tests"))
import colwinre               # noqa: E402

GUARD = 256
CASES = 60                    # argument sets per routine (the random number helpers: more)
HARNESS = os.path.join(ROOT, "build", "asm_harness.exe")

# name: (harness id, address, argument kinds)  p far pointer, l long, w word, b byte (pushed as
# a word); the random number helpers read their argument from the CALLER's frame ([bp+6]).
# A case's arguments may also say P (a far pointer at its block's start, offset 0) and E (EDX
# on entry, for the original only)
FUNCS = {
    "asm_wipe_rows":        (0, "1090:0000", "pppppp" + "w" * 9),
    "asm_copy_bits":        (1, "1090:00ff", "pp" + "w" * 10),
    "asm_fill_rect":        (2, "1090:01b4", "p" + "w" * 7),
    "asm_draw_sprite":      (3, "1090:0215", "pppb" + "w" * 12),
    "asm_draw_mono_sprite": (4, "1090:0348", "pppb" + "w" * 12 + "b"),
    "asm_decode_delta_flc": (5, "1080:0000", "ppwwwb"),
    "asm_decode_byte_run":  (6, "1080:00e4", "ppwwwb"),
    "asm_decode_literal":   (7, "1080:0168", "ppwwwb"),
    "asm_lzw_decode":       (8, "1088:0000", "pppwwww"),
    "L17_01dc":             (9, "1080:01dc", "W"),       # random_below(n): n at the caller's [bp+6]
    "L17_01f0":             (10, "1080:01f0", "L"),      # set the seed: a long at the caller's [bp+6]
    "L17_01fe":             (11, "1080:01fe", ""),       # the seed
}
RETURNS = {"asm_lzw_decode": 16, "L17_01dc": 16, "L17_01fe": 32}


# ------------------------------------------------------------------ inputs

def noise(r, n):
    return bytes(r.getrandbits(8) for _ in range(n))


def bitmap(r, w, h):
    """(stride, bytes): a bitmap at least w wide, its stride's sign a coin toss (<= 0: bottom-up)"""
    stride = w + r.choice([0, 0, 1, 3, 4, 17])
    return (stride if r.random() < 0.5 else -stride), noise(r, stride * h)


def span(r, size, edge):
    """(start, length) inside 0..size: at the start, at the end, the whole, or anywhere"""
    if edge == 0:
        return 0, r.randint(1, size)
    if edge == 1:
        n = r.randint(1, size)
        return size - n, n
    if edge == 2:
        return 0, size
    a = r.randrange(size)
    return a, r.randint(1, size - a)


def case_fill(r, k):
    w, h = r.randint(1, 96), r.randint(1, 64)
    stride, bits = bitmap(r, w, h)
    left, cw = span(r, w, k % 4)
    top, ch = span(r, h, (k // 4) % 4)
    return [bits], [("p", 0), ("w", r.randrange(256)), ("w", left), ("w", top), ("w", cw), ("w", ch),
                    ("w", stride), ("w", h)]


def case_copy(r, k):
    sw, sh, dw, dh = r.randint(1, 96), r.randint(1, 64), r.randint(1, 96), r.randint(1, 64)
    ss, sbits = bitmap(r, sw, sh)
    ds, dbits = bitmap(r, dw, dh)
    w, h = r.randint(1, min(sw, dw)), r.randint(1, min(sh, dh))
    edge = k % 4
    sx = [0, sw - w, 0, r.randint(0, sw - w)][edge]
    sy = [0, sh - h, sh - h, r.randint(0, sh - h)][edge]
    dx = [dw - w, 0, 0, r.randint(0, dw - w)][edge]
    dy = [dh - h, 0, dh - h, r.randint(0, dh - h)][edge]
    return [sbits, dbits], [("p", 0), ("p", 1), ("w", sx), ("w", sy), ("w", dx), ("w", dy), ("w", w), ("w", h),
                            ("w", sh), ("w", dh), ("w", ss), ("w", ds)]


def case_wipe(r, k):
    """wipe_rows: rows come from tbl (words, from row i, `step` of them), columns from tbl + h
    BYTES; each pixel's row is (trow + col) % h, offset through 32-bit row tables"""
    w, h = r.randint(1, 48), r.randint(1, 40)
    left, top, x, y = r.randint(0, 8), r.randint(0, 8), r.randint(0, 8), r.randint(0, 8)
    sw, dw = left + w + r.randint(0, 4), x + w + r.randint(0, 4)
    srows_n, drows_n = top + h, y + h
    sstride, dstride = sw + r.randint(0, 3), dw + r.randint(0, 3)
    src, dst = noise(r, sstride * srows_n), noise(r, dstride * drows_n)
    order = [list(range(srows_n)), list(range(srows_n))[::-1], r.sample(range(srows_n), srows_n)][k % 3]
    # the original reads ds:[row + left + j] and writes es:[...]: the row tables are offsets in
    # the bitmaps' segments and the far pointers' offsets are not added (the game's are 0, WinG
    # DIBs); so both pointers point at their blocks' starts, and the rows count the guard in
    src_rows = b"".join(struct.pack("<I", GUARD + o * sstride) for o in order)
    dorder = [list(range(drows_n)), r.sample(range(drows_n), drows_n), list(range(drows_n))][k % 3]
    dst_rows = b"".join(struct.pack("<I", GUARD + o * dstride) for o in dorder)
    i, step = r.randint(0, 20), r.randint(1, 20)
    tbl_len = max(2 * (i + step), h + 2 * w) + 8
    tbl = b"".join(struct.pack("<H", r.getrandbits(16)) for _ in range(tbl_len // 2))
    key = r.choice([-1, -1, src[r.randrange(len(src))], 0])
    cols = noise(r, 2 * w)                      # passed, never read
    return [src, dst, cols, tbl, src_rows, dst_rows], [
        ("P", 0), ("P", 1), ("p", 2), ("p", 3), ("p", 4), ("p", 5),
        ("w", i), ("w", w), ("w", h), ("w", step), ("w", left), ("w", top), ("w", x), ("w", y), ("w", key & 0xFFFF)]


def scale_map(r, n, src_n, base=0):
    """a monotonic map of n output positions onto src_n source ones (a scale), from base"""
    num, den = src_n, max(n, 1)
    if r.random() < 0.3:
        return [base + min(src_n - 1, i) for i in range(n)]             # 1:1
    return [base + min(src_n - 1, (i * num) // den) for i in range(n)]


def case_sprite(r, k, mono=False):
    """a sprite of R records {start, count, bytes}; the shared map scales rows from (dy + sy) and
    columns from (dx + sx); the output is rows x cols at (px, py) of the destination"""
    R, SW = r.randint(1, 24), r.randint(1, 40)
    row0 = r.choice([0, 0, r.randint(0, 5)])
    key = r.randrange(256)
    recs = b""
    for _ in range(R):
        start = r.randrange(SW)
        count = r.choice([0, r.randint(0, SW - start)]) if r.random() < 0.2 else r.randint(1, SW - start)
        px = bytes(key if r.random() < 0.25 else r.getrandbits(8) for _ in range(count))
        recs += struct.pack("<HH", start, count) + px
    recs += struct.pack("<HH", 0, 0)
    rows, cols = r.randint(1, 30), r.randint(1, 30)
    W, H = cols + r.randint(0, 20), rows + r.randint(0, 20)
    stride, bits = bitmap(r, W, H)
    px_, py_ = [(0, 0), (W - cols, H - rows), (0, H - rows), (r.randint(0, W - cols), r.randint(0, H - rows))][k % 4]
    dy, sy, dx, sx = r.randint(0, 6), r.randint(0, 6), r.randint(0, 6), r.randint(0, 6)
    # one table: rows read map[dy + sy ...], columns map[dx + sx ...]; build it so both hold
    n = max(dy + sy + rows, dx + sx + cols) + 4
    rmap = scale_map(r, rows, R, row0)
    cmap = scale_map(r, cols, SW)
    m = [0] * n
    clash = set(range(dy + sy, dy + sy + rows)) & set(range(dx + sx, dx + sx + cols))
    if clash:                                   # shared slots: shift the columns past the rows
        dx += dy + sy + rows - (dx + sx)
        n = max(n, dx + sx + cols + 4)
        m = [0] * n
    for j, v in enumerate(rmap):
        m[dy + sy + j] = v
    for j, v in enumerate(cmap):
        m[dx + sx + j] = v
    mapb = b"".join(struct.pack("<h", v) for v in m)
    args = [("p", 0), ("p", 1), ("p", 2), ("b", key), ("w", px_), ("w", py_), ("w", stride), ("w", H),
            ("w", cols), ("w", rows), ("w", dx), ("w", dy), ("w", r.getrandbits(16)), ("w", row0), ("w", sx), ("w", sy)]
    if mono:
        args.append(("b", r.randrange(256)))
    return [recs, bits, mapb], args


def case_delta(r, k):
    """DELTA_FLC: a line count, then per line skip words (negative) or a packet count, each packet
    {column skip, count: > 0 copies count words, < 0 repeats a word}; a count of 0 is a packet
    too (the decoder tests the count after it)"""
    w, h = r.randint(2, 80), r.randint(1, 40)
    stride = w + r.choice([0, 0, 2, 7])
    dst = noise(r, stride * h)
    out, lines, y = b"", 0, 0
    while y < h:
        if r.random() < 0.2 and y < h - 1:
            n = r.randint(1, h - 1 - y)
            out += struct.pack("<h", -n)
            y += n
            continue
        packets, x, body = [], 0, b""
        while x < w - 1 and len(packets) < 8 and r.random() < 0.85:
            skip = r.randint(0, min(10, w - 2 - x))
            room = (w - x - skip) // 2
            if room < 1:
                break
            cnt = r.randint(1, min(room, 20))
            if r.random() < 0.5:
                body += bytes([skip, cnt]) + noise(r, 2 * cnt)
            else:
                body += bytes([skip, (-cnt) & 0xFF]) + noise(r, 2)
            packets.append(cnt)
            x += skip + 2 * cnt
        if not packets:                         # a packet count of 0 still reads one packet
            body = bytes([0, 1]) + noise(r, 2)
        out += struct.pack("<H", len(packets)) + body
        lines += 1
        y += 1
    src = struct.pack("<H", lines) + out
    return [src, dst], [("p", 0), ("p", 1), ("w", w), ("w", h), ("w", stride), ("b", r.randrange(256))]


def case_byte_run(r, k):
    """BYTE_RUN: per row a packet count byte (not read), then counts: > 0 repeats a byte, < 0
    copies; the row gap is added to the source as well (kept), so rows carry that padding"""
    w, h = r.randint(1, 80), r.randint(1, 40)
    stride = w + (0 if k % 2 else r.randint(1, 6))
    gap = stride - w
    src = b""
    for _ in range(h):
        row, left = bytes([r.randrange(256)]), w
        while left > 0:
            n = r.randint(1, min(left, 127))
            if r.random() < 0.5:
                row += bytes([n, r.randrange(256)])
            else:
                row += bytes([(-n) & 0xFF]) + noise(r, n)
            left -= n
        src += row + noise(r, gap)
    return [src, noise(r, stride * h)], [("p", 0), ("p", 1), ("w", w), ("w", h), ("w", stride), ("b", r.randrange(256))]


def case_literal(r, k):
    """FLI_COPY. The original moves the destination on by EDX after each row, a register it never
    sets: what its caller left there. The port moves it by stride - w; the original is run with
    EDX = stride - w (an "E" argument: a register, not a stack word), so the rest is compared"""
    w, h = r.randint(1, 80), r.randint(1, 40)
    stride = w + (0 if k % 2 else r.randint(1, 6))
    return [noise(r, stride * h), noise(r, stride * h)], [("p", 0), ("p", 1), ("w", w), ("w", h), ("w", stride),
                                                          ("b", r.randrange(256)), ("E", stride - w)]


def lzw_encode(pixels, code_size, clear_every=None):
    """GIF LZW, codes LSB first, in sub-blocks of <= 255 bytes, then a 0 block"""
    clear, eoi = 1 << code_size, (1 << code_size) + 1
    codes, table, nbits, nxt = [], {}, code_size + 1, eoi + 1
    out = [(clear, nbits)]
    cur, emitted = None, 0
    for p in pixels:
        if cur is None:
            cur = (p,)
            continue
        if cur + (p,) in table:
            cur = cur + (p,)
            continue
        out.append((table[cur] if len(cur) > 1 else cur[0], nbits))
        emitted += 1
        if nxt < 4096:
            table[cur + (p,)] = nxt
            nxt += 1
            if nxt > (1 << nbits) and nbits < 12:
                nbits += 1
        if (clear_every and emitted % clear_every == 0) or nxt >= 4096:
            out.append((clear, nbits))
            table, nbits, nxt = {}, code_size + 1, eoi + 1
        cur = (p,)
    if cur is not None:
        out.append((table[cur] if len(cur) > 1 else cur[0], nbits))
    # the decoder adds an entry after every code but the first, so after the last one it holds
    # as many as this side and widens on reaching the next power of two, not past it
    if nxt >= (1 << nbits) and nbits < 12:
        nbits += 1
    out.append((eoi, nbits))
    acc = n = 0
    data = bytearray()
    for c, b in out:
        acc |= c << n
        n += b
        while n >= 8:
            data.append(acc & 0xFF)
            acc >>= 8
            n -= 8
    if n:
        data.append(acc & 0xFF)
    blocks = bytearray()
    for i in range(0, len(data), 255):
        chunk = data[i:i + 255]
        blocks += bytes([len(chunk)]) + chunk
    return bytes(blocks) + b"\0"


def case_lzw(r, k):
    code_size = [2, 3, 4, 8, 8, 5][k % 6]
    w, h = r.randint(1, 120), r.randint(1, 90)
    stride = w + r.choice([0, 0, 1, 3])
    colours = 1 << code_size
    pal = r.sample(range(colours), min(colours, r.choice([2, 3, 8, colours])))
    pixels, run = [], None
    for _ in range(w * h):                       # runs, so strings repeat and KwKwK happens
        if run is None or r.random() < 0.3:
            run = r.choice(pal)
        pixels.append(run)
    src = lzw_encode(pixels, code_size, clear_every=r.choice([None, None, 300]))
    base = r.randrange(256) if code_size < 8 else 0
    tables = (struct.pack("<H", 0xFFFF) * 0x1000 + b"".join(struct.pack("<H", (base + i) & 0xFFFF) for i in range(0x1000))
              + struct.pack("<H", 1) * 0x1000)
    bits = noise(r, stride * h + 4)
    return [src, bits, tables], [("p", 0), ("p", 1), ("p", 2), ("w", w), ("w", h), ("w", stride), ("w", code_size)]


def case_random_below(r, k):
    n = [0, 1, 2, 3, 7, 100, 0x7FFF, 0x8000, 0xFFFF, r.getrandbits(16)][k % 10]
    return [], [("W", n)]


def case_set_seed(r, k):
    return [], [("L", r.getrandbits(32))]


GENERATORS = {
    "asm_wipe_rows": case_wipe, "asm_copy_bits": case_copy, "asm_fill_rect": case_fill,
    "asm_draw_sprite": case_sprite, "asm_draw_mono_sprite": lambda r, k: case_sprite(r, k, True),
    "asm_decode_delta_flc": case_delta, "asm_decode_byte_run": case_byte_run, "asm_decode_literal": case_literal,
    "asm_lzw_decode": case_lzw, "L17_01dc": case_random_below, "L17_01f0": case_set_seed,
    "L17_01fe": lambda r, k: ([], []),
}


def cases(only=None):
    out = []
    for name in FUNCS:
        if only and name != only:
            continue
        r = random.Random("asm:" + name)
        n = CASES * (4 if name.startswith("L17") else 1)
        for k in range(n):
            bufs, args = GENERATORS[name](r, k)
            guarded = [noise(r, GUARD) + b + noise(r, GUARD) for b in bufs]
            out.append({"fn": name, "seed": r.getrandbits(32), "bufs": guarded, "args": args})
    return out


# ------------------------------------------------------------------ the game's assets

def ne_resources(path):
    """[(type name or ordinal, id, body)]: an NE file's resources, string types named through its
    RT_NAMETABLE (type 15) as KERNEL resolved them (src/host/ne_res.c)"""
    b = open(path, "rb").read()
    o = struct.unpack_from("<I", b, 0x3C)[0]
    if b[o:o + 2] != b"NE":
        return []
    rt, rn = struct.unpack_from("<HH", b, o + 0x24)
    if rt == rn:
        return []
    p = o + rt
    align = struct.unpack_from("<H", b, p)[0]
    p += 2
    res = []
    while True:
        rtype, count = struct.unpack_from("<HH", b, p)
        if rtype == 0:
            break
        p += 8
        for _ in range(count):
            off, ln, _fl, rid = struct.unpack_from("<HHHH", b, p)
            p += 12
            res.append((rtype & 0x7FFF, rid & 0x7FFF, b[off << align:(off << align) + (ln << align)]))
    names = {}
    for tord, _rid, body in res:
        if tord != 15:
            continue
        q = 0
        while q + 6 <= len(body):
            size, t, _i = struct.unpack_from("<HHH", body, q)
            if not size:
                break
            tname = body[q + 6:].split(b"\0")[0].decode("latin-1")
            names[t & 0x7FFF] = tname
            q += size
    return [(names.get(t, str(t)), rid, body) for t, rid, body in res]


def scale_table(num, den):
    """SetGlobalSpriteScale's map: 0x400 output positions onto source ones"""
    m, step = [], 0
    if num >= den:
        acc = num
        while len(m) < 0x400:
            while acc >= den and len(m) < 0x400:
                m.append(step)
                acc -= den
            acc += num
            step += 1
    else:
        acc = 0
        while len(m) < 0x400:
            while acc >= 0 and len(m) < 0x400:
                m.append(step)
                acc -= den
            acc += num
            step += 1
    return m


def scale_mult(m, num, den, amount, start, exact):
    """ScaleMult"""
    if amount == 0:
        return 0
    neg, amount = amount < 0, abs(amount)
    i, limit = start, m[start] + amount
    while m[i] < limit:
        i += 1
    if num < den and not exact and m[i] != limit:
        i -= 1
    return (-1 if neg else 1) * (i - start)


# the scales the game sets (SetGlobalSpriteScale's constant callers; the map zoom's z/8; one
# report's (100 / (tier * 2)) / 100), 1:1 apart
SCALES = [(7, 8), (3, 4), (8, 7), (2, 1), (4, 5), (3, 8), (1, 2)] + [(z, 8) for z in range(1, 17) if z != 8] + [
    (100 // (2 * t), 100) for t in (1, 2, 3, 4)]


def sprite_draws(body, r, k):
    """Sprite_DrawSprite's calls for one SPRT resource (Sprite_Init's layout: hotspot, bounds,
    area, key, size, rows): 1:1 and mono 1:1 inside the canvas, then two of the game's other
    scales in turn, clipped at the top left of a bottom-up canvas and at the bottom right"""
    hx, hy = struct.unpack_from("<hh", body, 0)
    bl, bt, br, bb, al, at, ar, ab = struct.unpack_from("<8h", body, 4)
    key = body[0x14]
    size = struct.unpack_from("<I", body, 0x15)[0]
    data = body[0x19:0x19 + size]
    out = []
    a, b = SCALES[k % len(SCALES)], SCALES[(k * 7 + 3) % len(SCALES)]
    for num, den, where, orient, mono in ((1, 1, "in", 1, False), (a[0], a[1], "tl", -1, False),
                                          (b[0], b[1], "br", 1, False), (1, 1, "in", 1, True)):
        if (br - bl) * num // den > 0x400 or (bb - bt) * num // den > 0x400:
            continue
        m = scale_table(num, den)
        inset_x, inset_y = al - bl, at - bt
        try:
            sx, sy = scale_mult(m, num, den, inset_x, 0, 1), scale_mult(m, num, den, inset_y, 0, 1)
            shx, shy = scale_mult(m, num, den, hx, 0, 0), scale_mult(m, num, den, hy, 0, 0)
            W, H = scale_mult(m, num, den, ar - al, sx, 0), scale_mult(m, num, den, ab - at, sy, 0)
        except IndexError:
            continue
        pw, ph = max(W, 1) + 16, max(H, 1) + 16
        tx = {"in": 8, "tl": -(W // 3), "br": pw - (2 * W) // 3}[where]
        ty = {"in": 8, "tl": -(H // 3), "br": ph - (2 * H) // 3}[where]
        ax, ay = tx, ty                            # = ScaleMult(hx) + px + sx, px chosen for it
        left, top, right, bottom = max(ax, 0), max(ay, 0), min(ax + W, pw), min(ay + H, ph)
        if right <= left or bottom <= top:
            continue                               # IntersectRect: nothing to draw, no call
        dx, dy = left - ax if left > ax else 0, top - ay if top > ay else 0
        stride = (pw + 3) & ~3
        args = [("p", 0), ("p", 1), ("p", 2), ("b", key), ("w", left), ("w", top), ("w", stride * orient), ("w", ph),
                ("w", right - left), ("w", bottom - top), ("w", dx), ("w", dy), ("w", inset_x), ("w", inset_y),
                ("w", sx), ("w", sy)]
        if mono:
            args.append(("b", r.randrange(256)))
        mapb = b"".join(struct.pack("<h", v) for v in m)
        out.append(("asm_draw_mono_sprite" if mono else "asm_draw_sprite", "%d:%d %s" % (num, den, where),
                    [data, noise(r, stride * ph), mapb], args))
    return out


def canvas_decode(body, r):
    """GRPort_LoadPicture -> MSPicDecompress: pixels after the palette, code size = bpp, the
    tables' suffixes from `first` (0 here: the opening's)"""
    w, h = (body[0] << 8) | body[1], (body[2] << 8) | body[3]
    bpp, mask = body[4], body[5]
    pix = body[6 + (mask + 1) * 3:]
    stride = (w + 3) & ~3
    tables = (struct.pack("<H", 0xFFFF) * 0x1000 + b"".join(struct.pack("<H", i) for i in range(0x1000))
              + struct.pack("<H", 1) * 0x1000)
    return ("asm_lzw_decode", "%dx%d bpp %d" % (w, h, bpp), [pix, noise(r, stride * h), tables],
            [("p", 0), ("p", 1), ("p", 2), ("w", w), ("w", h), ("w", stride), ("w", bpp)])


def flic_chunks(body, r):
    """(frames, [calls]): Flic_DecodeFrame's assembly chunks, every frame of one FLIC"""
    _size, magic, nframes, w, h = struct.unpack_from("<IHHHH", body, 0)
    p = (struct.unpack_from("<I", body, 80)[0] if magic == 0xAF12 else 0) or 128
    stride = (w + 3) & ~3
    n, calls = 0, []
    while p + 16 <= len(body) and n < nframes:
        fsize, fmagic, nchunks = struct.unpack_from("<IHH", body, p)
        if fmagic == 0xF100:
            p += fsize
            continue
        if fmagic != 0xF1FA:
            break
        q = p + 16
        for _ in range(nchunks):
            csize, ctype = struct.unpack_from("<IH", body, q)
            chunk = body[q + 6:q + csize]
            name = {7: "asm_decode_delta_flc", 15: "asm_decode_byte_run", 16: "asm_decode_literal"}.get(ctype)
            if name:
                args = [("p", 0), ("p", 1), ("w", w), ("w", h), ("w", stride), ("b", 0)]
                if ctype == 16:
                    args.append(("E", stride - w))
                calls.append((name, "frame %d chunk %d" % (n, ctype), [chunk, noise(r, stride * h)], args))
            q += csize
        p += fsize
        n += 1
    return n, calls


def asset_cases():
    r = random.Random("asm:assets")
    d = colwinre.game_dir()
    out, counts = [], {"SPRT": 0, "CVPC": 0, "FLIC": 0, "FLIC frames": 0, "sprites drawn": 0}
    for f in sorted(os.listdir(d)):
        if not f.upper().endswith((".DLL", ".EXE")):
            continue
        for tname, rid, body in ne_resources(os.path.join(d, f)):
            t = tname.upper()
            if t == "SPRT":
                counts["SPRT"] += 1
                draws = sprite_draws(body, r, counts["SPRT"])
                counts["sprites drawn"] += bool(draws)
            elif t == "CVPC":
                counts["CVPC"] += 1
                draws = [canvas_decode(body, r)]
            elif t == "FLIC":
                counts["FLIC"] += 1
                frames, draws = flic_chunks(body, r)
                counts["FLIC frames"] += frames
            else:
                continue
            for fn, what, bufs, args in draws:
                guarded = [noise(r, GUARD) + b + noise(r, GUARD) for b in bufs]
                out.append({"fn": fn, "seed": 0, "bufs": guarded, "args": args,
                            "asset": "%s %s %d %s" % (f, t, rid, what), "kind": t})
    return out, counts


# ------------------------------------------------------------------ the port's side

def build_harness():
    srcs = [os.path.join(ROOT, "tests", "asm", "harness.c")] + sorted(
        os.path.join(ROOT, "src", "asm", f) for f in os.listdir(os.path.join(ROOT, "src", "asm")) if f.endswith(".c"))
    if os.path.exists(HARNESS) and os.path.getmtime(HARNESS) > max(os.path.getmtime(s) for s in srcs):
        return
    os.makedirs(os.path.dirname(HARNESS), exist_ok=True)
    # as tools/undefined.py compiles the port
    r = subprocess.run(["i686-w64-mingw32-gcc", "-std=gnu99", "-O0", "-g", "-w", "-fno-omit-frame-pointer",
                        "-static", "-o", HARNESS] + srcs, capture_output=True, text=True, stdin=subprocess.DEVNULL)
    if r.returncode:
        sys.exit("the asm harness does not build:\n" + r.stderr[-2000:])


def native(calls, tmp):
    build_harness()
    job, res = os.path.join(tmp, "native.job"), os.path.join(tmp, "native.out")
    with open(job, "wb") as f:
        f.write(struct.pack("<I", len(calls)))
        for c in calls:
            f.write(struct.pack("<III", FUNCS[c["fn"]][0], c["seed"], len(c["bufs"])))
            for b in c["bufs"]:
                f.write(struct.pack("<I", len(b)) + b + b"\0" * (-len(b) % 4))
            args = [a for a in c["args"] if a[0] != "E"]
            f.write(struct.pack("<I", len(args)))
            for kind, v in args:
                if kind == "p":
                    f.write(struct.pack("<III", 2, v, GUARD))
                elif kind == "P":
                    f.write(struct.pack("<III", 2, v, 0))
                elif kind in "lL":
                    f.write(struct.pack("<III", 1, v & 0xFFFFFFFF, 0))
                else:
                    f.write(struct.pack("<III", 0, v & 0xFFFF, 0))
    wine = next((w for w in ("wine", "wine64") if subprocess.run(["which", w], capture_output=True).returncode == 0), None)
    r = subprocess.run([wine, HARNESS, "Z:" + job.replace("/", "\\"), "Z:" + res.replace("/", "\\")],
                       capture_output=True, text=True, env=dict(os.environ, WINEDEBUG="-all"))
    if r.returncode or not os.path.exists(res):
        sys.exit("the asm harness failed (%d): %s" % (r.returncode, r.stderr[-1500:]))
    out, data, p = [], open(res, "rb").read(), 0
    for c in calls:
        status, ret, seed = struct.unpack_from("<III", data, p)
        p += 12
        bufs = []
        for b in c["bufs"]:
            bufs.append(data[p:p + len(b)])
            p += len(b)
        out.append({"ok": status == 0, "ret": ret, "seed": seed, "bufs": bufs})
    return out


# ------------------------------------------------------------------ the original's side

def emulate_main(job, res):
    t = colwinre.use()
    os.chdir(t)
    import win16emu as W
    calls = pickle.load(open(job, "rb"))
    m = W.Machine()
    m.setup()
    m.relocate_all()
    slots = []                                   # (selector, linear base, capacity), reused
    out = []
    for c in calls:
        m.reset_log()
        m.scratch_next = W.SCRATCH_LO
        ptrs = []
        for i, b in enumerate(c["bufs"]):
            if i >= len(slots) or slots[i][2] < len(b):
                cap = max(len(b), 0x10000)
                sel, base, _n = m.alloc_huge(cap, "asm.py buffer %d" % i)
                if i < len(slots):
                    slots[i] = (sel, base, cap)
                else:
                    slots.append((sel, base, cap))
            sel, base, _cap = slots[i]
            m.uc.mem_write(base, bytes(b))
            ptrs.append((sel, base))
        m.write(W.DGROUP, 0x3F00, struct.pack("<I", c["seed"]))
        words, regs = [], {}
        for kind, v in c["args"]:
            if kind == "p":
                words += [GUARD, ptrs[v][0]]
            elif kind == "P":
                words += [0, ptrs[v][0]]
            elif kind == "E":
                regs["edx"] = v & 0xFFFFFFFF
            elif kind == "l":
                words += [v & 0xFFFF, (v >> 16) & 0xFFFF]
            elif kind in "wb":
                words.append(v & 0xFFFF)
            elif kind == "W":                    # in the caller's frame: [bp+6]
                regs["bp"] = m.alloc(struct.pack("<H", v & 0xFFFF)) - 6
            elif kind == "L":
                regs["bp"] = m.alloc(struct.pack("<I", v & 0xFFFFFFFF)) - 6
        o = m.call_far(FUNCS[c["fn"]][1], words, max_instr=200_000_000, regs=regs)
        out.append({"ok": bool(o["reached_return"]), "error": o.get("error") or o.get("stall"),
                    "ret": o["return_dxax"], "seed": struct.unpack("<I", bytes(m.read(W.DGROUP, 0x3F00, 4)))[0],
                    "bufs": [bytes(m.uc.mem_read(base, len(b))) for (sel, base), b in zip(ptrs, c["bufs"])]})
    pickle.dump(out, open(res, "wb"))
    return 0


def emulate(calls, tmp):
    job, res = os.path.join(tmp, "emu.job"), os.path.join(tmp, "emu.out")
    pickle.dump(calls, open(job, "wb"))
    r = subprocess.run([colwinre.python(), __file__, "--emulate", job, res], capture_output=True, text=True)
    if r.returncode:
        sys.exit("the emulator side failed:\n" + r.stderr[-3000:])
    return pickle.load(open(res, "rb"))


# ------------------------------------------------------------------ compared

def compare(c, e, n):
    diffs = []
    if not e["ok"]:
        diffs.append("the original did not return (%s)" % (e.get("error"),))
    if not n["ok"]:
        diffs.append("the port's faulted")
    if diffs:
        return diffs
    bits = RETURNS.get(c["fn"])
    if bits and (e["ret"] ^ n["ret"]) & ((1 << bits) - 1):
        diffs.append("return %#x, port %#x" % (e["ret"] & ((1 << bits) - 1), n["ret"] & ((1 << bits) - 1)))
    if e["seed"] != n["seed"]:
        diffs.append("seed %#010x, port %#010x" % (e["seed"], n["seed"]))
    for i, (a, b) in enumerate(zip(e["bufs"], n["bufs"])):
        if a != b:
            k = next(j for j in range(len(a)) if a[j] != b[j])
            where = "guard before" if k < GUARD else "guard after" if k >= len(a) - GUARD else "+%#x" % (k - GUARD)
            diffs.append("buffer %d differs at %s (%02x, port %02x; %d bytes differ)"
                         % (i, where, a[k], b[k], sum(x != y for x, y in zip(a, b))))
    return diffs


def run_assets():
    calls, counts = asset_cases()
    with tempfile.TemporaryDirectory() as tmp:
        emu = emulate(calls, tmp)
        nat = native(calls, tmp)
    per = {}
    for c, e, n in zip(calls, emu, nat):
        d = compare(c, e, n)
        st = per.setdefault(c["kind"], {"calls": 0, "identical": 0, "first": None})
        st["calls"] += 1
        if d:
            st["first"] = st["first"] or c["asset"] + ": " + "; ".join(d)
        else:
            st["identical"] += 1
    print("assets: %d SPRT (%d drawn), %d CvPc, %d FLIC (%d frames)" % (
        counts["SPRT"], counts["sprites drawn"], counts["CVPC"], counts["FLIC"], counts["FLIC frames"]))
    for k, st in sorted(per.items()):
        print("%-5s %-9s %d/%d calls identical%s" % (k, "identical" if st["calls"] == st["identical"] else "DIFFERS",
                                                    st["identical"], st["calls"], "" if not st["first"] else "\n    first: " + st["first"]))
    return counts, per


def main(argv):
    if argv[:1] == ["--emulate"]:
        return emulate_main(argv[1], argv[2])
    if "--assets" in argv and "--record" not in argv:
        _c, per = run_assets()
        return 0 if all(s["calls"] == s["identical"] for s in per.values()) else 1
    only = argv[argv.index("--only") + 1] if "--only" in argv else None
    calls = cases(only)
    with tempfile.TemporaryDirectory() as tmp:
        emu = emulate(calls, tmp)
        nat = native(calls, tmp)
    per = {}
    for c, e, n in zip(calls, emu, nat):
        d = compare(c, e, n)
        st = per.setdefault(c["fn"], {"sets": 0, "agree": 0, "first": None})
        st["sets"] += 1
        if d:
            if st["first"] is None:
                st["first"] = "; ".join(d) + "  args " + " ".join("%s%x" % (k, v) for k, v in c["args"])
        else:
            st["agree"] += 1
    ok = 0
    for name in FUNCS:
        if name not in per:
            continue
        st = per[name]
        good = st["agree"] == st["sets"]
        ok += good
        print("%-22s %-7s %d/%d argument sets%s" % (name, "agree" if good else "DIFFERS", st["agree"], st["sets"],
                                                     "" if good else "\n    first: " + st["first"]))
    print("%d of %d routines agree" % (ok, len(per)))
    if "--record" in argv and not only:
        counts, assets = run_assets()
        with open(os.path.join(ROOT, "status", "asm.json"), "w") as f:
            json.dump({"routines": len(per), "agree": ok,
                       "functions": {n: {"sets": s["sets"], "agree": s["agree"]} for n, s in per.items()},
                       "assets": {k: {"calls": s["calls"], "identical": s["identical"]} for k, s in assets.items()},
                       "asset_counts": counts}, f, indent=1, sort_keys=True)
            f.write("\n")
        ok_assets = all(s["calls"] == s["identical"] for s in assets.values())
        return 0 if ok == len(per) and ok_assets else 1
    return 0 if ok == len(per) else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
