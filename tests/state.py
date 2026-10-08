#!/usr/bin/env python3
"""The game's globals, between the native build and the shipped program's 16-bit memory (W3).

    python3 tests/state.py --roundtrip      the gate: 16-bit -> native -> 16-bit, and back, lossless
    python3 tests/state.py --record         also write status/state.json (tools/status.py reads it)

Every global the model addresses (col-win-re's analysis/model.json, through
import/layout.json) has two homes: a segment and offset in the shipped program, where the
emulator holds it, and a native address in the port, which build/port_state.exe reports
(tests/state/build.py). This file converts between them, global by global, element by
element, from the port's own declaration of each one:

  far pointer    4 bytes selector:offset <-> a native address, through the two address maps:
                 a pointer into a global maps to the same element and byte of it there. Null
                 is null. A pointer into no global (the heap, a handle) cannot be mapped, and
                 says so.
  int/unsigned   Borland's 2 bytes <-> the port's 4 where the port still declares `int`:
                 sign- or zero-extended in, and out only if the value fits.
  same width     the bytes, unchanged: records the port lays out like the game (packed, i16).
  other          unconvertible, and listed by name: a class whose native layout differs.

The 16-bit state comes from the emulator after loading a real save ($COL_GAME_DIR/AUTO01.SAV)
-- col-win-re's probe.build('save'), run in the unicorn venv (tests/colwinre.py). The round
trip loads it into the native build (port_state.exe roundtrip, under Wine), reads it back,
converts it to 16 bits again and compares, global by global; then converts that native image
to 16 bits and back and compares again. Lossless means every convertible global comes back
byte for byte; the gate also requires every unconvertible one to be listed.
"""
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
sys.path.insert(0, os.path.join(ROOT, "tests"))
import colwinre               # noqa: E402
import gen_globals as gg      # noqa: E402

EXE = os.path.join(ROOT, "build", "port_state.exe")
INT_TYPES = {"int": True, "signed": True, "signed int": True, "short": True, "i16": True, "int16_t": True,
             "long": True, "i32": True, "int32_t": True,
             "unsigned": False, "unsigned int": False, "unsigned short": False, "u16": False, "uint16_t": False,
             "unsigned long": False, "u32": False, "uint32_t": False}


def sel_of(seg):
    return 0x1000 + (seg - 1) * 8


def seg_of(sel):
    return (sel - 0x1000) // 8 + 1 if sel >= 0x1000 and (sel - 0x1000) % 8 == 0 else None


# ------------------------------------------------------------------ the description

def describe():
    """{name: codec} for each global both sides have. A codec: seg, off, count, w16 (one
    element in the game), wn (natively), addr (native), kind, signed."""
    layout = json.load(open(os.path.join(ROOT, "import", "layout.json")))
    lay = harness("layout")
    native = {n: (a, e, c) for n, a, e, c in lay["globals"]}
    shapes = ClassShapes(lay["fields"])
    decls = dict(gg.declarations())
    decls.update(gg.declarations(cpp=True))      # C++ sees the class where C sees a byte view
    for n, d in gg.file_only().items():
        decls.setdefault(n, d)
    out = {}
    addressed = dict(json.load(open(os.path.join(ROOT, "import", "addresses.json"))))
    addressed.update(layout["globals"])
    for name, (seg, off) in addressed.items():
        if name in native and native[name][2] == 0:
            # known by address only: a pointer to its start maps, nothing is converted
            out[name] = {"seg": seg, "off": off, "count": 0, "w16": 1, "wn": 1, "addr": native[name][0],
                         "kind": "address", "signed": False, "elem16": 1, "leaves": [], "type": "?"}
            continue
        if name not in native or name not in decls:
            continue
        addr, wn, count = native[name]
        d = decls[name]
        m = gg.VAR.match(d)
        if m:
            base = re.sub(r"\s+", " ", m.group(1).replace("const ", "")).strip()
            stars = (m.group(2) or "").replace(" ", "")
            dims = re.findall(r"\[([^\]]*)\]", m.group(4))
        else:                                   # a function pointer
            base, stars, dims = "fnptr", "*", []
        inner = 1
        for x in dims[1:]:
            inner *= int(x, 0)
        w16 = gg.game_width(base, stars, layout["records"])
        if w16:
            w16 *= inner
        special = None
        if base in shapes.cls and not stars and inner == 1:
            special = shapes.leaves(base)
            w16 = wn - 2 * sum(1 for _o, k in special if k == "vptr")
        if base in FN_POINTERS and not stars:
            stars = "*"
            w16 = 4
        if name in NOT_POINTERS:
            kind, w16 = "bytes", wn
        elif special is not None and not special:
            kind = "bytes"
        elif special is not None:
            kind = "class"
        elif stars and not inner > 1:
            kind = "pointer"
        elif base in INT_TYPES and w16 and wn != w16:
            kind = "int"
        elif w16 and w16 == wn:
            kind = "bytes"
        else:
            kind = "other"
        out[name] = {"seg": seg, "off": off, "count": count, "w16": w16, "wn": wn, "addr": addr,
                     "kind": kind, "signed": INT_TYPES.get(base, False), "elem16": (w16 or 0) // max(inner, 1),
                     "leaves": special or [], "type": base}
    return out


FN_POINTERS = {"WNDPROC", "FARPROC", "DLGPROC"}

# Globals the corpus types as a far pointer that hold no pointer: a four-character resource
# type (the MS framework's Mac-style OSType), copied into a local and passed by address
# (CTable_Init: `type = g_color_resource_type; MSGetResource(&type, id)`). The saved
# bytes are "CTAB", "FLIC", "PICT"/"cPic", "TEXT". Converted as 4 bytes.
NOT_POINTERS = {"g_color_resource_type", "g_flic_resource_type", "g_picture_resource_type",
                "g_restype_text_popup"}


class ClassShapes:
    """Each class's native shape (from the harness's field table) and its special leaves:
    the vtable pointer (4 bytes natively, Borland's near 2 in the game) and pointer members."""

    def __init__(self, rows):
        import importlib.util
        spec = importlib.util.spec_from_file_location("state_build", os.path.join(ROOT, "tests", "state", "build.py"))
        b = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(b)
        self.cls = b.classes()
        self.size, self.base, self.member = {}, {}, {}
        for c, what, n, off, size in rows:
            if what == "size":
                self.size[c] = off
            elif what == "base":
                self.base[(c, n)] = off
            else:
                self.member[(c, n)] = (off, size)

    def poly(self, c):
        bases, _m, own = self.cls[c]
        return own or any(self.poly(b) for b in bases if b in self.cls)

    def leaves(self, c, at=0):
        """[(native offset, 'vptr' | 'ptr')] in class c placed at `at`."""
        bases, members, _own = self.cls[c]
        out = []
        if self.poly(c) and not any(self.poly(b) for b in bases if b in self.cls):
            out.append((at, "vptr"))          # GCC: a class with no polymorphic base holds it at 0
        for b in bases:
            if b in self.cls:
                out += self.leaves(b, at + self.base[(c, b)])
        for t, n, dims in members:
            off, size = self.member[(c, n)]
            count = 1
            for x in re.findall(r"\[([^\]]*)\]", dims):
                count *= int(eval(x.replace("0x", "0x"), {}))
            esize = size // count
            for i in range(count):
                if t in self.cls:
                    out += self.leaves(t, at + off + i * esize)
                elif "*" in t:
                    out.append((at + off + i * esize, "ptr"))
        return sorted(out)


# ------------------------------------------------------------------ pointers

class AddressMap:
    def __init__(self, codecs, literals=None):
        self.c = [(n, c) for n, c in codecs.items() if c["w16"]]
        # pointers to what no global covers (string literals), paired by the value both
        # programs start the same pointer global with
        self.lit16 = dict(literals or {})
        self.litn = {v: k for k, v in self.lit16.items()}

    def to_native(self, far):
        if far == 0:
            return 0, None
        if far in self.lit16:
            return self.lit16[far], None
        off, sel = far & 0xFFFF, far >> 16
        seg = seg_of(sel)
        for n, c in self.c:
            if c["kind"] == "address":
                if c["seg"] == seg and c["off"] == off:
                    return c["addr"], None
                continue
            if c["seg"] == seg and c["off"] <= off < c["off"] + c["count"] * c["w16"]:
                i, r = divmod(off - c["off"], c["w16"])
                if r and c["w16"] != c["wn"]:
                    return None, "points into %s[%d] at +%d, laid out differently natively" % (n, i, r)
                return c["addr"] + i * c["wn"] + r, None
        return None, "points to %04x:%04x, in no global" % (sel, off)

    def to_16(self, p):
        if p == 0:
            return 0, None
        if p in self.litn:
            return self.litn[p], None
        for n, c in self.c:
            if c["kind"] == "address":
                if c["addr"] == p:
                    return (sel_of(c["seg"]) << 16) | c["off"], None
                continue
            if c["addr"] <= p < c["addr"] + c["count"] * c["wn"]:
                i, r = divmod(p - c["addr"], c["wn"])
                if r and c["w16"] != c["wn"]:
                    return None, "points into %s[%d] at +%d, laid out differently in the game" % (n, i, r)
                return (sel_of(c["seg"]) << 16) | (c["off"] + i * c["w16"] + r), None
        return None, "points to %08x, in no global" % p


# ------------------------------------------------------------------ conversion

def to_native(name, c, b16, amap):
    """(native bytes, [why lossy])."""
    if c["kind"] == "bytes":
        return b16, []
    out, why = bytearray(), []
    for i in range(c["count"]):
        e = b16[i * c["w16"]:(i + 1) * c["w16"]]
        if c["kind"] == "pointer":
            p, w = amap.to_native(struct.unpack("<I", e)[0])
            if w:
                why.append("[%d] %s" % (i, w))
                p = 0
            out += struct.pack("<I", p)
        elif c["kind"] == "int":
            v = int.from_bytes(e, "little", signed=c["signed"])
            out += v.to_bytes(c["wn"], "little", signed=c["signed"])
        elif c["kind"] == "class":
            pos16 = 0
            prev = 0
            for off, k in c["leaves"]:
                # bytes up to the leaf are the same bytes, shifted by the vptrs before them
                gap = off - prev
                out += e[pos16:pos16 + gap]
                pos16 += gap
                if k == "vptr":
                    v = struct.unpack_from("<H", e, pos16)[0]
                    if v:
                        why.append("[%d] vptr %04x: no native vtable for it" % (i, v))
                    out += struct.pack("<I", 0)
                    pos16 += 2
                    prev = off + 4
                else:
                    p, w = amap.to_native(struct.unpack_from("<I", e, pos16)[0])
                    if w:
                        why.append("[%d] +%d %s" % (i, off, w))
                        p = 0
                    out += struct.pack("<I", p)
                    pos16 += 4
                    prev = off + 4
            out += e[pos16:]
        else:
            return None, ["unconvertible: %d bytes in the game, %d natively" % (c["w16"] or 0, c["wn"])]
    return bytes(out), why


def to_16(name, c, bn, amap):
    if c["kind"] == "bytes":
        return bn, []
    out, why = bytearray(), []
    for i in range(c["count"]):
        e = bn[i * c["wn"]:(i + 1) * c["wn"]]
        if c["kind"] == "pointer":
            f, w = amap.to_16(struct.unpack("<I", e)[0])
            if w:
                why.append("[%d] %s" % (i, w))
                f = 0
            out += struct.pack("<I", f)
        elif c["kind"] == "int":
            v = int.from_bytes(e, "little", signed=c["signed"])
            lo, hi = (-(1 << (8 * c["w16"] - 1)), (1 << (8 * c["w16"] - 1)) - 1) if c["signed"] else (0, (1 << (8 * c["w16"])) - 1)
            if not lo <= v <= hi:
                why.append("[%d] %d does not fit %d bytes" % (i, v, c["w16"]))
            out += (v & ((1 << (8 * c["w16"])) - 1)).to_bytes(c["w16"], "little", signed=False)
        elif c["kind"] == "class":
            prev = 0
            for off, k in c["leaves"]:
                out += e[prev:off]
                if k == "vptr":
                    v = struct.unpack_from("<I", e, off)[0]
                    if v:
                        why.append("[%d] vptr %08x: no game vtable for it" % (i, v))
                    out += struct.pack("<H", 0)
                else:
                    f, w = amap.to_16(struct.unpack_from("<I", e, off)[0])
                    if w:
                        why.append("[%d] +%d %s" % (i, off, w))
                        f = 0
                    out += struct.pack("<I", f)
                prev = off + 4
            out += e[prev:]
        else:
            return None, ["unconvertible"]
    return bytes(out), why


# ------------------------------------------------------------------ the two sides

def wine():
    for w in ("wine", "wine64"):
        p = shutil.which(w)
        if p:
            return p
    sys.exit("no wine on PATH: the native harness is a Win32 program")


def harness(*args):
    if not os.path.exists(EXE):
        subprocess.run([sys.executable, os.path.join(ROOT, "tests", "state", "build.py")], check=True)
    r = subprocess.run([wine(), EXE] + list(args), capture_output=True, text=True,
                       env=dict(os.environ, WINEDEBUG="-all"))
    if r.returncode:
        sys.exit("port_state %s failed (%d): %s" % (" ".join(args), r.returncode, r.stderr[-500:]))
    return json.loads(r.stdout) if args[0] == "layout" else r


def stage16(codecs, state="save"):
    """{name: 16-bit bytes} from the emulator with a real save loaded (in the unicorn venv)."""
    want = {n: [c["seg"], c["off"], c["count"] * c["w16"]] for n, c in codecs.items()
            if c["w16"] and c["kind"] != "address"}
    with tempfile.TemporaryDirectory() as tmp:
        spec, res = os.path.join(tmp, "want.json"), os.path.join(tmp, "got.json")
        json.dump(want, open(spec, "w"))
        r = subprocess.run([colwinre.python(), __file__, "--stage", spec, res, state], capture_output=True, text=True)
        if r.returncode:
            sys.exit("staging failed:\n" + r.stderr[-2000:])
        return {n: bytes.fromhex(h) for n, h in json.load(open(res)).items()}


def literals(codecs):
    """{game far pointer: native pointer} for pointer globals' initial values that point at
    no global: the shipped image (relocated) and the native executable start the same
    pointer global's elements with the same literals."""
    ptrs = {n: c for n, c in codecs.items() if c["kind"] == "pointer"}
    s16 = stage16(ptrs, state="shipped")
    with tempfile.TemporaryDirectory() as tmp:
        out = os.path.join(tmp, "init.bin")
        harness("dump", out)
        nat = blobs_read(out)
    plain = AddressMap(codecs)
    pairs = {}
    for n, c in ptrs.items():
        if n not in s16 or n not in nat:
            continue
        for i in range(c["count"]):
            far = struct.unpack_from("<I", s16[n], i * 4)[0]
            p = struct.unpack_from("<I", nat[n], i * 4)[0]
            if far and p and plain.to_native(far)[0] is None:
                pairs[far] = p
    return pairs


def stage_main(spec, res, state="save"):
    t = colwinre.use()
    os.chdir(t)
    import probe
    m, meta = probe.build(state, [])
    # the loader patches far pointers as segments are touched; a global whose initial value
    # is one (g_popup_label's string literals) still holds the 0xffff placeholder until then
    m.relocate_all()
    out = {}
    for n, (seg, off, size) in json.load(open(spec)).items():
        out[n] = bytes(m.read(seg, off, size)).hex()
    json.dump(out, open(res, "w"))
    return 0


def blobs_write(path, blobs):
    with open(path, "wb") as f:
        for n, b in sorted(blobs.items()):
            f.write(n.encode() + b"\0" + struct.pack("<I", len(b)) + b)


def blobs_read(path):
    data, out, i = open(path, "rb").read(), {}, 0
    while i < len(data):
        j = data.index(b"\0", i)
        n = data[i:j].decode()
        size = struct.unpack_from("<I", data, j + 1)[0]
        out[n] = data[j + 5:j + 5 + size]
        i = j + 5 + size
    return out


# ------------------------------------------------------------------ the gate

def roundtrip():
    codecs = describe()
    amap = AddressMap(codecs, literals(codecs))
    s16 = stage16(codecs)
    native, lossy_in, unconv = {}, {}, []
    for n, c in sorted(codecs.items()):
        if c["kind"] == "address":
            continue
        if n not in s16:
            unconv.append((n, "no game width"))
            continue
        b, why = to_native(n, c, s16[n], amap)
        if b is None:
            unconv.append((n, why[0]))
            continue
        native[n] = b
        if why:
            lossy_in[n] = why
    with tempfile.TemporaryDirectory() as tmp:
        a, b = os.path.join(tmp, "in.bin"), os.path.join(tmp, "out.bin")
        blobs_write(a, native)
        harness("roundtrip", a, b)
        back = blobs_read(b)
    lost16, lostn = {}, {}
    for n, bn in native.items():
        c = codecs[n]
        if back.get(n) != bn:
            lostn[n] = "the native build did not hold it"
            continue
        b16, why = to_16(n, c, back[n], amap)
        if n not in lossy_in and b16 != s16[n]:
            lost16[n] = why or ["bytes differ"]
        again, _w = to_native(n, c, b16, amap)
        if again != back[n] and n not in lossy_in:
            lostn.setdefault(n, "native -> 16-bit -> native differs")
    return codecs, s16, native, lossy_in, unconv, lost16, lostn


def main(argv):
    if argv[:1] == ["--stage"]:
        return stage_main(argv[1], argv[2], argv[3] if len(argv) > 3 else "save")
    if "--roundtrip" not in argv and "--record" not in argv:
        print(__doc__)
        return 2
    codecs, s16, native, lossy_in, unconv, lost16, lostn = roundtrip()
    kinds = {}
    for c in codecs.values():
        kinds[c["kind"]] = kinds.get(c["kind"], 0) + 1
    print("globals: %d both sides (%s)" % (len(codecs), ", ".join("%d %s" % (v, k) for k, v in sorted(kinds.items()))))
    print("converted: %d; unconvertible: %d; pointers that map to no global: %d" % (
        len(native), len(unconv), len(lossy_in)))
    print("round trip 16 -> native -> 16: %d lost; native -> 16 -> native: %d lost" % (len(lost16), len(lostn)))
    for n, w in sorted(lost16.items())[:20]:
        print("  lost 16: %-28s %s" % (n, w[0][:90]))
    for n, w in sorted(lostn.items())[:20]:
        print("  lost native: %-24s %s" % (n, w[:90]))
    if "-v" in argv:
        for n, w in unconv:
            print("  unconvertible: %-24s %s" % (n, w[:90]))
        for n, w in sorted(lossy_in.items()):
            print("  unmapped pointer: %-21s %s" % (n, w[0][:90]))
    ok = not lost16 and not lostn
    if "--record" in argv:
        out = {"globals": len(codecs), "converted": len(native), "lost": len(lost16) + len(lostn), "ok": ok,
               "unconvertible": sorted(n for n, _w in unconv), "unmapped_pointers": sorted(lossy_in),
               "address_only": sorted(n for n, c in codecs.items() if c["kind"] == "address"),
               "kinds": kinds}
        with open(os.path.join(ROOT, "status", "state.json"), "w") as f:
            json.dump(out, f, indent=1, sort_keys=True)
            f.write("\n")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
