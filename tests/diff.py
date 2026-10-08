#!/usr/bin/env python3
"""The per-function differential: the shipped function against the native one (roadmap W3).

    python3 tests/diff.py              compare each function in FUNCTIONS, every argument set
    python3 tests/diff.py --record     also write status/diff.json (tools/status.py reads it)

Each function runs twice from one state, a real save loaded into col-win-re's emulator:
  - the ORIGINAL, in the emulator, at its address in the shipped executable;
  - the PORT's, natively (build/port_state.exe under Wine), over the same globals,
    converted to the native layout by tests/state.py.
Before every call the state is restored. Compared: the return value (AX, or DX:AX for a
long, AL for a char) and every modelled global the call changed, in the game's 16-bit
layout -- the native globals are converted back first. A function agrees when every
argument set agrees.

The functions are leaves (they call nothing), C, integer arguments only, reading and
writing the game's globals: what can be compared before the platform layer exists.
"""
import json
import os
import re
import subprocess
import sys
import tempfile
import importlib.util

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tests"))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import colwinre               # noqa: E402

# game logic, each a leaf: cargo, units, colonies, nations, the map, plans, tribes
FUNCTIONS = ["get_cargo_amount", "set_cargo_amount", "top_unit", "bottom_unit", "next_unit",
             "is_colonist", "job_of_unit", "is_armed", "get_unit_dest", "field_index",
             "colony_has_building", "has_father", "get_treaty", "set_treaty", "freedom_level",
             "on_map", "which_move", "cargo_free", "tribe_radius", "region_build_factor"]
VALUES = [0, 1, 2, 3, 7, 12]
NATIONS = [0, 1, 2, 3]          # an index past the four nations reads past g_nations: each build
                                # reads its own neighbours, which compares nothing
# the state every call starts from, beyond the save: the selection the game has whenever it
# calls these (cargo_free reads the current colony's stock through g_cur_colony_rec)
STAGE = {"g_cur_colony_rec": ("g_colonies", 0)}


def state_module():
    spec = importlib.util.spec_from_file_location("state", os.path.join(ROOT, "tests", "state.py"))
    m = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(m)
    return m


def signature(name):
    """(address, [param types], return type) from src/*/<name>.c."""
    for layer in ("game", "gfx", "platform"):
        p = os.path.join(ROOT, "src", layer, name + ".c")
        if os.path.exists(p):
            t = open(p, errors="replace").read()
            addr = re.search(r"candidate for (10[0-9a-f]{2}:[0-9a-f]{4})", t).group(1)
            m = re.search(r"^([A-Za-z_][\w \t\*]*?)\b%s\s*\(([^)]*)\)\s*\{" % re.escape(name), t, re.M)
            params = [] if m.group(2).strip() in ("", "void") else [x.strip() for x in m.group(2).split(",")]
            return addr, params, m.group(1).strip()
    raise SystemExit("no source for %s" % name)


def domain(param):
    return NATIONS if "nation" in param else VALUES


def argsets(params):
    if not params:
        return [[]]
    if len(params) == 1:
        return [[v] for v in domain(params[0])]
    out = []
    for a in domain(params[0])[:3]:
        for b in domain(params[1])[:3]:
            out.append([a, b] + [1] * (len(params) - 2))
    return out


def ret_bits(rtype):
    r = rtype.replace("extern", "").replace('"C"', "").strip()
    if r == "void":
        return 0
    if "long" in r:
        return 32
    if "char" in r:
        return 8
    return 16


# ------------------------------------------------------------------ the emulator side

def emulate_main(job, res):
    t = colwinre.use()
    os.chdir(t)
    import probe
    spec = json.load(open(job))
    m, _meta = probe.build("save", [])
    m.relocate_all()
    for ptr, (target, index) in spec["stage"].items():
        seg, off, _s = spec["globals"][ptr]
        tseg, toff, w = spec["stage_targets"][target]
        m.write(seg, off, (((0x1000 + (tseg - 1) * 8) << 16) | (toff + index * w)).to_bytes(4, "little"))
    pre = {n: bytes(m.read(seg, off, size)) for n, (seg, off, size) in spec["globals"].items()}
    out = []
    for call in spec["calls"]:
        for n, (seg, off, _size) in spec["globals"].items():
            m.write(seg, off, pre[n])
        o = probe.observe(m, call["addr"], [a & 0xFFFF for a in call["args"]], 200000)
        post = {n: bytes(m.read(seg, off, size)) for n, (seg, off, size) in spec["globals"].items()}
        out.append({"ret": o.get("return_dxax"), "reached": o.get("reached_return"),
                    "error": o.get("error"),
                    "changed": {n: b.hex() for n, b in post.items() if b != pre[n]}})
    json.dump({"pre": {n: b.hex() for n, b in pre.items()}, "calls": out}, open(res, "w"))
    return 0


# ------------------------------------------------------------------ both, compared

def run():
    st = state_module()
    codecs = st.describe()
    amap = st.AddressMap(codecs, st.literals(codecs))
    globals16 = {n: [c["seg"], c["off"], c["count"] * c["w16"]] for n, c in codecs.items()
                 if c["w16"] and c["kind"] != "address"}
    calls = []
    for f in FUNCTIONS:
        addr, params, rtype = signature(f)
        for a in argsets(params):
            calls.append({"fn": f, "addr": addr, "args": a, "bits": ret_bits(rtype)})
    with tempfile.TemporaryDirectory() as tmp:
        job, res = os.path.join(tmp, "job.json"), os.path.join(tmp, "res.json")
        targets = {t: [codecs[t]["seg"], codecs[t]["off"], codecs[t]["w16"]] for t, _i in STAGE.values()}
        json.dump({"globals": globals16, "calls": calls, "stage": STAGE, "stage_targets": targets}, open(job, "w"))
        r = subprocess.run([colwinre.python(), __file__, "--emulate", job, res], capture_output=True, text=True)
        if r.returncode:
            sys.exit("emulator side failed:\n" + r.stderr[-2000:])
        emu = json.load(open(res))
        pre16 = {n: bytes.fromhex(h) for n, h in emu["pre"].items()}
        native = {}
        for n, b in pre16.items():
            nb, _why = st.to_native(n, codecs[n], b, amap)
            if nb is not None:
                native[n] = nb
        inp, jobs, outp = os.path.join(tmp, "in.bin"), os.path.join(tmp, "jobs.txt"), os.path.join(tmp, "out")
        st.blobs_write(inp, native)
        open(jobs, "w").write("".join("%s %s\n" % (c["fn"], " ".join(str(a) for a in c["args"])) for c in calls))
        out = st.harness("batch", inp, jobs, outp).stdout.splitlines()
        rets = {int(l.split()[0]): l.split()[2] for l in out if len(l.split()) == 3}
        results = []
        for k, c in enumerate(calls):
            e = emu["calls"][k]
            diffs = []
            if not e["reached"]:
                diffs.append("the original did not return (%s)" % e.get("error"))
            if rets.get(k) in (None, "unknown"):
                diffs.append("the native call did not run")
            elif rets.get(k) == "fault":
                diffs.append("the native call faulted (the original did not)")
            elif c["bits"] and e["ret"] is not None and rets[k] != "fault":
                mask = (1 << c["bits"]) - 1
                ne, nn = e["ret"] & mask, int(rets[k]) & mask
                if ne != nn:
                    diffs.append("return %#x, native %#x" % (ne, nn))
            post = st.blobs_read("%s.%d" % (outp, k)) if os.path.exists("%s.%d" % (outp, k)) else {}
            changed_n = {}
            for n, b in post.items():
                if n in native and b != native[n]:
                    b16, _w = st.to_16(n, codecs[n], b, amap)
                    changed_n[n] = b16
            changed_e = {n: bytes.fromhex(h) for n, h in e["changed"].items()}
            for n in sorted(set(changed_e) | set(changed_n)):
                if changed_e.get(n) != changed_n.get(n):
                    a, b = changed_e.get(n, pre16.get(n)), changed_n.get(n, pre16.get(n))
                    i = next((i for i in range(min(len(a), len(b))) if a[i] != b[i]), None)
                    diffs.append("%s differs%s" % (n, "" if i is None else " at +%#x (%02x, native %02x)" % (i, a[i], b[i])))
            results.append((c, diffs, sorted(changed_e)))
    return results


def main(argv):
    if argv[:1] == ["--emulate"]:
        return emulate_main(argv[1], argv[2])
    results = run()
    per = {}
    for c, diffs, changed in results:
        per.setdefault(c["fn"], []).append((c["args"], diffs, changed))
    agree = 0
    for f in FUNCTIONS:
        rows = per.get(f, [])
        bad = [r for r in rows if r[1]]
        writes = sorted({n for r in rows for n in r[2]})
        print("%-22s %s  %d argument sets%s" % (f, "agree" if not bad else "DIFFER", len(rows),
                                                "; writes " + ", ".join(writes) if writes else ""))
        for args, diffs, _c in bad[:3]:
            print("    %s: %s" % (args, "; ".join(diffs)[:160]))
        agree += not bad
    print("%d of %d functions agree" % (agree, len(FUNCTIONS)))
    if "--record" in argv:
        with open(os.path.join(ROOT, "status", "diff.json"), "w") as fh:
            json.dump({"total": len(FUNCTIONS), "agree": agree,
                       "functions": {f: not any(r[1] for r in per.get(f, [])) for f in FUNCTIONS}},
                      fh, indent=1, sort_keys=True)
            fh.write("\n")
    return 0 if agree == len(FUNCTIONS) else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
