#!/usr/bin/env python3
"""The framework call trace: the shipped game against the port, call for call (roadmap W4 gate).

    python3 tests/trace.py oracle [N]      the shipped COLONIZE.EXE in col-win-re's emulator
    python3 tests/trace.py native [N]      the port (an instrumented build) under Wine
    python3 tests/trace.py compare         how far the two agree
    python3 tests/trace.py --gate [N]      all three; --record writes status/trace.json

The game's framework is its MS* functions (src/platform and the per-module copies of them).
Win16 and Win32 API calls cannot be compared one to one, but the framework is the same code on
both sides, so its calls -- which function, with which arguments, in which order -- can. Both
runs start the game with no input; the first N (10,000) framework calls are recorded:
  oracle  every MS* address is observed in the emulator; arguments come off the 16-bit stack
          (a word for an int, two for a long or a far pointer);
  native  the port is built with -finstrument-functions and src/gen's table of the same
          functions (tools/undefined.py, PORT_INSTRUMENT=1); the entry hook reads the
          arguments from the function's frame, 4 bytes each.
They are compared in the game's terms: an int by its low 16 bits, a long whole, a pointer by
the global it points into (the models' addresses on one side, the executable's symbols on the
other) -- null, a global, or elsewhere (the stack, the heap), which only matches elsewhere.
"""
import bisect
import json
import os
import re
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tests"))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import colwinre               # noqa: E402

OUT = os.path.join(ROOT, "build", "trace")
N_DEFAULT = 10000


def framework():
    """[(name, 'SSSS:OOOO', kinds)] -- every MS* function src/ defines at a known address; kinds
    is one letter per parameter: i an int (a word), l a long, p a pointer."""
    import importlib.util
    spec = importlib.util.spec_from_file_location("port_twins", os.path.join(ROOT, "tools", "twins.py"))
    twins = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(twins)
    out = []
    for layer in ("platform", "gfx", "game"):
        d = os.path.join(ROOT, "src", layer)
        for f in sorted(os.listdir(d)):
            if not f.startswith("MS") or not f.endswith(".c"):
                continue
            t = open(os.path.join(d, f), errors="replace").read()
            m = re.search(r"candidate for (10[0-9a-f]{2}:[0-9a-f]{4})", t)
            name = f[:-2]
            if not m:
                continue
            _t, defs = twins.definitions_text(t)
            own = [x for x in defs if x[0] == name]
            if not own:
                continue
            head = twins.blank(t)[own[0][2]:own[0][3]].split("{")[0]
            params = re.search(r"\((.*)\)", head, re.S).group(1).strip()
            kinds = ""
            if params not in ("", "void"):
                for p in params.split(","):
                    p = p.strip()
                    pname = re.findall(r"\w+", p)[-1] if re.findall(r"\w+", p) else ""
                    if "*" in p:
                        kinds += "p"
                    elif re.search(r"\blong\b", p):
                        kinds += "l"
                    elif re.search(r"\bchar\b", p):
                        kinds += "c"            # pushed as a word, its high byte whatever was there
                    elif (re.match(r"h(dc|wnd|menu|mem|res|bm|bitmap|pal|font|file|inst|andle|cursor|icon|obj|rsrc|lib)$", pname)
                          or "handle" in pname
                          or (pname == "h" and not re.search(r"\bw\b", params))):   # h beside w is a height
                        kinds += "h"            # a handle: compared by its order of first use
                    else:
                        kinds += "i"
            out.append((name, m.group(1), kinds))
    return out


# ------------------------------------------------------------------ pointers, canonical

def game_globals():
    """[(segment, offset, name, size)] sorted: where each modelled global is in the game, and how
    many bytes it has there (element width times count, from the port's declaration; an open
    array runs to the next global)."""
    import gen_globals as gg
    layout = json.load(open(os.path.join(ROOT, "import", "layout.json")))
    addr = dict(json.load(open(os.path.join(ROOT, "import", "addresses.json"))))
    addr.update(layout["globals"])
    ext = gg.extents(dict(layout, globals=addr))
    decls = dict(gg.declarations())
    decls.update(gg.declarations(cpp="all"))
    out = []
    for n, (seg, off) in addr.items():
        size = 2
        m = gg.VAR.match(decls.get(n, "")) if decls.get(n) else None
        if m:
            base, stars = m.group(1).strip(), (m.group(2) or "").replace(" ", "")
            dims = re.findall(r"\[([^\]]*)\]", m.group(4))
            w = gg.game_width(base, stars, layout["records"]) or 2
            for x in dims[1:]:
                w *= int(x, 0)
            count = 1
            if dims:
                count = int(dims[0], 0) if dims[0].strip() else max(1, ext.get(n, w) // w)
            size = w * count
        out.append((seg, off, n, size))
    return sorted(out)


def canon_far(far, table, keys, sp=None):
    if far == 0:
        return "0"
    sel, off = far >> 16, far & 0xFFFF
    # Win16's stack is in DGROUP (SS = DS): an offset at or above SP is a local, not a global
    if sp is not None and sel == 0x1000 + (34 - 1) * 8 and off >= sp:
        return "*"
    if sel < 0x1000 or (sel - 0x1000) % 8:
        return "*"
    seg = (sel - 0x1000) // 8 + 1
    i = bisect.bisect_right(keys, (seg, off, "￿")) - 1
    if i >= 0 and table[i][0] == seg and off < table[i][1] + table[i][3]:
        return table[i][2]
    return "*"


# ------------------------------------------------------------------ the oracle

def oracle_main(n, out):
    fns = framework()
    t = colwinre.use()
    os.chdir(t)
    sys.path.insert(0, os.path.join(t, "tools"))
    import e2e_turns as E
    table = game_globals()
    keys = [(s, o, n2) for s, o, n2, _sz in table]
    calls = []
    r = E.Recorder(os.path.join(t, "game", "COLONIZE.EXE"), turns=1, progress=False)

    def observer(name, kinds):
        def fn(machine, raw):
            if len(calls) >= n:
                return
            from unicorn.x86_const import UC_X86_REG_SP
            sp = machine.uc.reg_read(UC_X86_REG_SP)
            args, k = [], 0
            for c in kinds:
                if c in "ihc":
                    v = int.from_bytes(raw[k:k + 2], "little")
                    args.append("h%x" % v if c == "h" else "%d" % (v & 0xFF if c == "c" else v))
                    k += 2
                elif c == "l":
                    args.append(str(int.from_bytes(raw[k:k + 4], "little")))
                    k += 4
                else:
                    args.append(canon_far(int.from_bytes(raw[k:k + 4], "little"), table, keys, sp))
                    k += 4
            calls.append(name + " " + " ".join(args))
            if len(calls) >= n:
                machine.uc.emu_stop()
        return fn
    for name, addr, kinds in fns:
        r.m.on_call(addr, observer(name, kinds), name)
    while len(calls) < n and not r.t.stopped and not r.g.finished and r.m.log["instr"] < 3_000_000_000:
        r.run(5_000_000)
    with open(out, "w") as f:
        f.write("\n".join(calls) + "\n")
    print("oracle: %d framework calls, %d instructions" % (len(calls), r.m.log["instr"]))
    return 0


def oracle(n):
    os.makedirs(OUT, exist_ok=True)
    out = os.path.join(OUT, "oracle.txt")
    r = subprocess.run([colwinre.python(), __file__, "--oracle-run", str(n), out])
    return out if r.returncode == 0 else None


# ------------------------------------------------------------------ the port

def native(n):
    os.makedirs(OUT, exist_ok=True)
    env = dict(os.environ, PORT_INSTRUMENT="1")
    r = subprocess.run([sys.executable, os.path.join(ROOT, "tools", "undefined.py")], env=env,
                       capture_output=True, text=True)
    if "links" not in r.stdout:
        sys.exit("the instrumented build does not link:\n" + r.stdout[-1500:])
    raw = os.path.join(OUT, "native.raw")
    if os.path.exists(raw):
        os.remove(raw)
    env = dict(os.environ, PORT_MSTRACE="Z:" + raw.replace("/", "\\"), PORT_MSTRACE_N=str(n))
    subprocess.run([sys.executable, os.path.join(ROOT, "tools", "run_port.py"), "--seconds", "600"], env=env)
    # pointers: the executable's own data symbols, sorted
    sym = subprocess.run(["i686-w64-mingw32-nm", "-n", "--defined-only", os.path.join(ROOT, "build", "colonize.exe")],
                         capture_output=True, text=True).stdout.split("\n")
    data = []
    for line in sym:
        parts = line.split()
        if len(parts) == 3 and parts[1] in "bBdDrR" and parts[2].startswith("_"):
            data.append((int(parts[0], 16), parts[2][1:]))
    data.sort()
    addrs = [a for a, _n in data]
    kinds = {name: k for name, _a, k in framework()}
    out = os.path.join(OUT, "native.txt")
    lines = []
    for line in open(raw, errors="replace") if os.path.exists(raw) else []:
        parts = line.split()
        if not parts:
            continue
        name, vals = parts[0], [int(v, 16) for v in parts[1:]]
        args = []
        for c, v in zip(kinds.get(name, ""), vals):
            if c == "i":
                args.append(str(v & 0xFFFF))
            elif c == "h":
                args.append("h%x" % (v & 0xFFFF))
            elif c == "c":
                args.append(str(v & 0xFF))
            elif c == "l":
                args.append(str(v & 0xFFFFFFFF))
            elif v == 0:
                args.append("0")
            else:
                i = bisect.bisect_right(addrs, v) - 1
                args.append(data[i][1] if i >= 0 and data[i][1].startswith(("g_", "_ctype")) and v - addrs[i] < 0x10000 else "*")
        lines.append(name + " " + " ".join(args))
    with open(out, "w") as f:
        f.write("\n".join(lines) + "\n")
    print("native: %d framework calls" % len(lines))
    return out


def canon_handles(lines):
    """each handle (hNNNN) by its order of first use, so the two programs' numbering need not agree"""
    seen, out = {"h0": "0"}, []
    for l in lines:
        out.append(re.sub(r"\bh[0-9a-f]+\b", lambda m: seen.setdefault(m.group(0), "H%d" % len(seen)), l))
    return out


def compare(a, b):
    x = canon_handles([l.rstrip("\n") for l in open(a)])
    y = canon_handles([l.rstrip("\n") for l in open(b)])
    n = 0
    while n < min(len(x), len(y)) and x[n] == y[n]:
        n += 1
    return n, x, y


def main(argv):
    n = int(argv[argv.index("--n") + 1]) if "--n" in argv else N_DEFAULT
    if argv[:1] == ["--oracle-run"]:
        return oracle_main(int(argv[1]), argv[2])
    if argv[:1] == ["oracle"]:
        return 0 if oracle(n) else 1
    if argv[:1] == ["native"]:
        native(n)
        return 0
    if argv[:1] == ["compare"] or "--gate" in argv:
        if "--gate" in argv:
            oracle(n)
            native(n)
        a, b = os.path.join(OUT, "oracle.txt"), os.path.join(OUT, "native.txt")
        same, x, y = compare(a, b)
        print("matching prefix: %d of %d (oracle %d calls, native %d)" % (same, n, len(x), len(y)))
        if same < min(len(x), len(y)):
            for k in range(max(0, same - 3), min(same + 3, len(x), len(y))):
                print("  %s %6d  oracle: %s" % ("=" if x[k] == y[k] else "!", k, x[k][:100]))
                print("  %s %6d  native: %s" % (" ", k, y[k][:100]))
        if "--record" in argv:
            with open(os.path.join(ROOT, "status", "trace.json"), "w") as f:
                json.dump({"n": n, "matching": same, "oracle": len(x), "native": len(y)}, f, indent=1, sort_keys=True)
                f.write("\n")
        return 0 if same >= n else 1
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
