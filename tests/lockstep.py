#!/usr/bin/env python3
"""The autoplay lockstep: the shipped game and the port play the same saved game by themselves,
and their state is compared at every step of every turn (roadmap W6).

    python3 tests/lockstep.py oracle SAVE [--turns N]    record the original  -> build/lockstep/<name>.oracle.*
    python3 tests/lockstep.py native SAVE [--turns N]    record the port      -> build/lockstep/<name>.native.*
    python3 tests/lockstep.py compare SAVE               the two, checkpoint by checkpoint
    python3 tests/lockstep.py compare SAVE --full        every modelled global, not only the save's
    python3 tests/lockstep.py run SAVE [--turns N]       all three
    ... compare|run SAVE --record                      also merge the result into status/lockstep.json

A scenario is a saved game (SAVE: a .SAV path, or a name in $COL_GAME_DIR such as AUTO01.SAV).
Both sides start it the same way, through the game's own menu: the title, then BEGINMENU's
fourth entry (Load), whose file dialog is answered with SAVE without showing one (the oracle's
winsim and the port's shim each fill the name in and return OK). From then on nobody plays:
every nation is set to the computer and g_autoplay on, as the Cheat menu's Human entry does.

The original runs in col-win-re's emulator under its recorder (tests/oracle/e2e_turns.py, the
W3 oracle), which takes a CHECKPOINT at the first API call after (turn, GameBrains.step)
changes -- the natives, the four viceroys, the end of turn -- and records the 57 blocks
save_game writes plus the generators' state. The port takes its checkpoints by the same rule
(src/host/w16_lockstep.c) and dumps its globals; this file converts them to the 16-bit layout
(tests/state.py) and assembles the same blocks.

Time is not shared: the emulator's clock is its own and the port's is virtual. What the game
derives from the clock is the generators' seeds, and those are the oracle's, handed to the port
in order: srand's (the C runtime) and L17_01f0's (the assembly generator at DGROUP:0x3f00).
"""
import gzip
import json
import os
import shutil
import struct
import subprocess
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tests"))
import colwinre               # noqa: E402

OUT = os.path.join(ROOT, "build", "lockstep")

# the menu path to a saved game: the intro, left to end by itself (the port's does by tick
# ~135,000 of its clock; a key to skip it would land on whichever screen is up), then BEGINMENU's
# fourth entry (begin_game: 4 is LoadSaveGame). Waits in the emulator's ticks; the port posts each
# key when its game sits idle (w16_lockstep.c). The script ends at the Load key: the load and the
# turns run in the recorder's autoplay loop, where its turn limit holds -- a wait in ticks would
# not end while the game plays turns that make no API call, and the clock moves only on API calls.
LOAD_SETUP = [
    ("boot", "wait:180000"),
    ("begin_menu", "key:Down,wait:5000,key:Down,wait:5000,key:Down,wait:5000,key:Return,wait:1000"),
]
L17_SET_SEED = "1080:01f0"    # the assembly generator's seed: a long at the caller's [bp+6]


def scenario(save):
    """(name, absolute path) of a saved game"""
    path = save if os.path.isabs(save) or os.sep in save else os.path.join(colwinre.game_dir(), save)
    if not os.path.exists(path):
        sys.exit("no saved game %s" % path)
    return os.path.splitext(os.path.basename(path))[0], os.path.abspath(path)


# ------------------------------------------------------------------ the original

def oracle_main(save, turns, out, raw):
    """In the emulator's Python: the recorder, started from SAVE"""
    t = colwinre.use()
    os.chdir(t)
    shutil.copyfile(os.path.join(ROOT, "tests", "oracle", "e2e_turns.py"), os.path.join(t, "tools", "e2e_turns.py"))
    import e2e_turns as E
    E.SETUP = LOAD_SETUP
    r = E.Recorder(os.path.join(t, "game", "COLONIZE.EXE"), turns=turns, progress=True, raw_path=raw)
    sim, m = r.sim, r.m
    name = os.path.basename(save)
    game_file = sim.game_file
    sim.game_file = lambda base: save if base.upper() == name.upper() else game_file(base)

    def open_dialog(machine, stub, raw_args, ret, st):
        """COMMDLG.GetOpenFileName(OPENFILENAME far *): lpstrFile (+0x18) gets the name"""
        seg, off = sim.ptr(sim.argv(raw_args, 4)[0])
        ofn = sim.read(seg, off, 0x1c)
        fseg, foff = sim.ptr(struct.unpack_from("<I", ofn, 0x18)[0])
        sim.write(fseg, foff, name.encode() + b"\0")
        sim.log.append({"api": "COMMDLG.GETOPENFILENAME", "answered": name})
        return 1
    bound = False
    for lin, (mod, api) in sim._by_trap.items():
        if mod.upper() == "COMMDLG" and api.upper() == "GETOPENFILENAME":
            m.stubs[lin]["handler"] = r._wrap(open_dialog, "COMMDLG.GETOPENFILENAME")
            bound = True
    if not bound:
        sys.exit("the game imports no COMMDLG.GetOpenFileName")
    l17 = []

    def on_l17(machine, _raw):
        from unicorn.x86_const import UC_X86_REG_BP, UC_X86_REG_SS
        bp, ss = machine.uc.reg_read(UC_X86_REG_BP), machine.uc.reg_read(UC_X86_REG_SS)
        v = struct.unpack("<I", bytes(machine.uc.mem_read(machine.seg_base(ss) + bp + 6, 4)))[0]
        l17.append({"api_calls": r.api_n, "phase": r.phase, "seed": v})
    m.on_call(L17_SET_SEED, on_l17, "L17_01f0")

    # checkpoints at the writes of GameBrains.step, where the port takes them (w16_lockstep_step,
    # after each such statement), not at the next API call: the recorder's rule never fires, this
    # hook takes the snapshot with the step being written. The recorder's own step hook (the
    # all-AI stores) was added first and runs first, as the port's stores precede its dump.
    from unicorn import UC_HOOK_MEM_WRITE
    r.game_key = lambda: r.key
    lin = r.base[E.SEG20] + E.BRAINS + 2

    loaded = r.base[E.DGROUP] + 0x8B             # g_loaded_flag
    # and every data segment whole, beside the recorder's blocks: a divergence starts in state
    # the save does not hold (the map's visible range, a window's size) before it reaches it
    layout = json.load(open(os.path.join(ROOT, "import", "layout.json")))
    data_segs = {int(k): v for k, v in layout["segments"].items() if 20 <= int(k) <= 34}
    full = []

    def on_step(uc, _access, address, size, value, _ud):
        # the end of a human turn's store, with the recorder's all-AI ones: GameBrains_DoHuman
        # clears g_loaded_flag and no one else does, so a loaded game left with it set never
        # lets the viceroys before its turn_nation play again (GameBrains_DoViceroy)
        if struct.unpack("<h", bytes(uc.mem_read(r.base[E.SEG20] + E.GAME + 66, 2)))[0]:
            uc.mem_write(loaded, b"\0")
        new = bytearray(uc.mem_read(lin, 2))
        at = address - lin
        data = (value & ((1 << (8 * size)) - 1)).to_bytes(size, "little")
        new[at:at + size] = data[:2 - at]
        step, = struct.unpack("<h", new)
        turn, = struct.unpack("<h", bytes(uc.mem_read(r.base[E.SEG20] + E.GAME + 14, 2)))
        if (turn, step) != r.key:
            r.checkpoint((turn, step))
            segs = {i: bytearray(m.read(i, 0, n)) for i, n in data_segs.items()}
            segs[E.SEG20][E.BRAINS + 2:E.BRAINS + 4] = new
            full.append({i: bytes(b) for i, b in segs.items()})
            if r.halt:                           # the turn limit: now, not at the next API call
                uc.emu_stop()
            if r.raw is not None:
                b = bytearray.fromhex(r.raw[-1]["brains"])
                b[2:4] = new
                r.raw[-1]["brains"] = b.hex()
    r.uc.hook_add(UC_HOOK_MEM_WRITE, on_step, None, lin, lin + 1)
    doc = r.record()
    import pickle
    with gzip.open(raw.replace(".raw.gz", ".full.gz"), "wb") as fz:
        pickle.dump(full, fz)
    doc["scenario"] = name
    doc["l17_seeds"] = l17
    json.dump(doc, open(out, "w"), indent=1)
    return 0


def oracle(save, turns):
    name, path = scenario(save)
    os.makedirs(OUT, exist_ok=True)
    out, raw, log = (os.path.join(OUT, name + ".oracle" + x) for x in (".json", ".raw.gz", ".log"))
    t0 = time.time()
    with open(log, "w") as f:
        code = subprocess.run([colwinre.python(), "-u", __file__, "--oracle-run", path, str(turns), out, raw],
                              stdout=f, stderr=subprocess.STDOUT).returncode
    if code:
        sys.exit("the oracle run failed (%d):\n%s" % (code, open(log).read()[-3000:]))
    d = json.load(open(out))
    print("oracle %s: %d checkpoints, %d srand + %d L17 seeds, %.0f s" % (
        name, len(d["checkpoints"]), len(d["seeds"]), len(d["l17_seeds"]), time.time() - t0))
    return out, raw


# ------------------------------------------------------------------ the port

KEYS = {"Return": 0x0D, "Down": 0x28, "Space": 0x20}


def setup_keys():
    """the keys of LOAD_SETUP, in order"""
    out = []
    for _phase, spec in LOAD_SETUP:
        for part in spec.split(","):
            k, _, v = part.partition(":")
            if k == "key":
                out.append(KEYS[v])
    return out


def wine_path(p):
    return "Z:" + os.path.abspath(p).replace("/", "\\")


def native(save, turns, seconds=1800):
    name, path = scenario(save)
    d = json.load(open(os.path.join(OUT, name + ".oracle.json")))
    work = os.path.join(OUT, name + ".native")
    os.makedirs(work, exist_ok=True)
    for old in ("native.cp", "native.log"):
        if os.path.exists(os.path.join(work, old)):
            os.remove(os.path.join(work, old))
    lines = ["save " + wine_path(path), "turns %d" % turns]
    lines += ["key %#x" % k for k in setup_keys()]
    lines += ["nudge %d" % n["checkpoint"] for n in d["nudges"]]
    lines += ["srand %d" % x["seed"] for x in d["seeds"]]
    lines += ["l17 %d" % x["seed"] for x in d["l17_seeds"]]
    open(os.path.join(work, "lockstep.txt"), "w").write("\n".join(lines) + "\n")
    r = subprocess.run([sys.executable, os.path.join(ROOT, "tools", "undefined.py")], capture_output=True, text=True)
    if "links" not in r.stdout:
        sys.exit("the port does not link:\n" + r.stdout[-1500:])
    t0 = time.time()
    # the oracle's display, 640 x 480: the screen metrics and WM_GETMINMAXINFO say so in a lockstep
    # run (src/host), which is what places a maximized window; the 640 x 480 Wine desktop does the
    # same where the driver has virtual desktops (winemac does not)
    subprocess.run([sys.executable, os.path.join(ROOT, "tools", "run_port.py"), "--seconds", str(seconds),
                    "--desktop", "640x480"], env=dict(os.environ, PORT_LOCKSTEP=wine_path(work)))
    cps = native_checkpoints(os.path.join(work, "native.cp"))
    print("native %s: %d checkpoints, %.0f s (log: %s)" % (name, len(cps), time.time() - t0,
                                                            os.path.relpath(os.path.join(work, "native.log"), ROOT)))
    return cps


def native_checkpoints(path):
    """[{turn, step, rand, l17, globals: [(seg, off, count, w16, wn, kind, bytes)], planes: [bytes]}]"""
    if not os.path.exists(path):
        return []
    b, p, out = open(path, "rb").read(), 0, []
    while p + 2 <= len(b) and b[p:p + 2] == b"CP":
        i, turn, step, rand, l17, n = struct.unpack_from("<6I", b, p + 2)
        p += 26
        gl = []
        for _ in range(n):
            seg, off, count, w16, wn, kind = struct.unpack_from("<6H", b, p)
            p += 12
            gl.append((seg, off, count, w16, wn, kind, b[p:p + count * wn]))
            p += count * wn
        planes = []
        for _ in range(4):
            ln, = struct.unpack_from("<I", b, p)
            planes.append(b[p + 4:p + 4 + ln])
            p += 4 + ln
        out.append({"i": i, "turn": struct.unpack("<h", struct.pack("<H", turn & 0xFFFF))[0],
                    "step": struct.unpack("<h", struct.pack("<H", step & 0xFFFF))[0],
                    "rand": rand, "l17": l17, "globals": gl, "planes": planes})
    return out


def image16(cp):
    """{segment: (bytes, known mask)}: the port's globals in the game's 16-bit layout. An int is its
    low w16 bytes; a record or byte array is its bytes where the port lays it out as the game;
    a pointer, or a layout the port does not share, is unknown"""
    segs = {}
    for seg, off, count, w16, wn, kind, data in cp["globals"]:
        img, known = segs.setdefault(seg, (bytearray(0x10000), bytearray(0x10000)))
        for k in range(count):
            at, el = off + k * w16, data[k * wn:(k + 1) * wn]
            if at + w16 > 0x10000:
                break
            if kind in (1, 2):
                img[at:at + w16] = el[:w16]
            elif kind == 0 and w16 == wn:
                img[at:at + w16] = el
            else:
                continue
            known[at:at + w16] = b"\1" * w16
    return segs


def native_blocks(cp, E):
    """the recorder's fields (e2e_turns.STATE and PLANES) from a port checkpoint: {field: (bytes, mask)}"""
    segs = image16(cp)
    out = {}
    game = segs.get(E.SEG20, (bytearray(0x10000),))[0][E.GAME:E.GAME + 0x8e]
    for name, idx, off, size in E.STATE:
        if isinstance(size, tuple):
            cnt, = struct.unpack_from("<h", game, size[0])
            size = min(max(0, cnt) * size[1], 0x10000 - off)
        img, known = segs.get(idx, (bytearray(0x10000), bytearray(0x10000)))
        out[name] = (bytes(img[off:off + size]), bytes(known[off:off + size]))
    out["rtl_rand"] = (struct.pack("<I", cp["rand"]), b"\1" * 4)
    for (name, _ptr), pl in zip(E.PLANES, cp["planes"]):
        out[name] = (pl, b"\1" * len(pl))
    return out


def recorder_module():
    t = colwinre.tree()
    import importlib.util
    spec = importlib.util.spec_from_file_location("e2e_turns", os.path.join(ROOT, "tests", "oracle", "e2e_turns.py"))
    E = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(E)
    E.REPO = t                                   # model.json, for naming a record's member
    return E


def name_offset(E, field, off):
    rec = E.RECORDS.get(field)
    if not rec:
        return "%s+%#x" % (field, off)
    record, stride = rec
    return "%s[%d].%s" % (field, off // stride, E.member_at(record, off % stride))


def compare(save, quiet=False):
    """(checkpoints in lockstep, checkpoints compared, the first divergence or None)"""
    name, _path = scenario(save)
    E = recorder_module()
    d = json.load(open(os.path.join(OUT, name + ".oracle.json")))
    raw = json.load(gzip.open(os.path.join(OUT, name + ".oracle.raw.gz"), "rt"))
    ora = [(cp["turn"], cp["step"], blocks) for cp, blocks in zip(d["checkpoints"], raw)]
    nat = native_checkpoints(os.path.join(OUT, name + ".native", "native.cp"))
    # the oracle's machine also writes GameBrains.step before a game exists (its constructor's
    # stores): its run is compared from the checkpoint where the port's starts
    if nat:
        k0 = next((k for k, o in enumerate(ora) if (o[0], o[1]) == (nat[0]["turn"], nat[0]["step"])), None)
        if k0 is None:
            first = "the oracle never reaches the port's first checkpoint (turn %d step %d)" % (nat[0]["turn"], nat[0]["step"])
            if not quiet:
                print("%s: 0 checkpoints in lockstep\n  first divergence: %s" % (name, first))
            return 0, 0, first
        ora = ora[k0:]
    same, first = 0, None
    for k, (o, n) in enumerate(zip(ora, nat)):
        if (o[0], o[1]) != (n["turn"], n["step"]):
            first = "checkpoint %d: the oracle is at turn %d step %d, the port at turn %d step %d" % (
                k, o[0], o[1], n["turn"], n["step"])
            break
        nb = native_blocks(n, E)
        diffs = []
        for field, hexb in o[2].items():
            ob = bytes.fromhex(hexb)
            pb, known = nb.get(field, (b"", b""))
            if len(pb) != len(ob):
                diffs.append("%s: %d bytes, the port's %d" % (field, len(ob), len(pb)))
                continue
            for i in range(len(ob)):
                if known[i] and ob[i] != pb[i]:
                    diffs.append("%s: %02x, the port %02x" % (name_offset(E, field, i), ob[i], pb[i]))
                    break
        if diffs:
            first = "checkpoint %d (turn %d step %d): %s" % (k, o[0], o[1], "; ".join(diffs[:6]) +
                                                             (" (+%d more fields)" % (len(diffs) - 6) if len(diffs) > 6 else ""))
            break
        same += 1
    total = min(len(ora), len(nat))
    if not quiet:
        print("%s: %d of %d checkpoints in lockstep (oracle %d, port %d)" % (name, same, total, len(ora), len(nat)))
        if first:
            print("  first divergence: " + first)
    return same, total, first


def compare_full(save, limit=12):
    """every global the port dumps, in the 16-bit layout, against the oracle's data segments, at
    each checkpoint: the first checkpoint where any differs, and which"""
    import pickle
    name, _path = scenario(save)
    layout = json.load(open(os.path.join(ROOT, "import", "layout.json")))["globals"]
    names = {}
    for n, (seg, off) in layout.items():
        names.setdefault((seg, off), n)
    full = pickle.load(gzip.open(os.path.join(OUT, name + ".oracle.full.gz"), "rb"))
    d = json.load(open(os.path.join(OUT, name + ".oracle.json")))
    keys = [(c["turn"], c["step"]) for c in d["checkpoints"]]
    nat = native_checkpoints(os.path.join(OUT, name + ".native", "native.cp"))
    k0 = next((k for k, key in enumerate(keys) if key == (nat[0]["turn"], nat[0]["step"])), 0) if nat else 0
    for k, n in enumerate(nat):
        if k0 + k >= len(full):
            break
        o = full[k0 + k]
        diffs = []
        for seg, off, count, w16, wn, kind, data in n["globals"]:
            if kind == 3 or (kind == 0 and w16 != wn) or seg not in o:
                continue
            for e in range(count):
                at, el = off + e * w16, data[e * wn:(e + 1) * wn]
                ob = o[seg][at:at + w16]
                pb = el[:w16]
                if len(ob) == w16 and ob != pb:
                    i = next(j for j in range(w16) if ob[j] != pb[j])
                    g = names.get((seg, off), "%d:%04x" % (seg, off))
                    diffs.append("%s%s+%#x (%d:%04x): %s, the port %s" % (
                        g, "[%d]" % e if count > 1 else "", i, seg, at + i, ob[i:i + 4].hex(), pb[i:i + 4].hex()))
                    break
        if diffs:
            print("checkpoint %d (turn %d step %d): %d globals differ" % (k, n["turn"], n["step"], len(diffs)))
            for x in diffs[:limit]:
                print("  " + x)
            return k, diffs
    print("every dumped global agrees at every checkpoint")
    return None, []


def record(save, same, total, first):
    """merge one scenario's result into status/lockstep.json (a ledger: other scenarios stay)"""
    name, path = scenario(save)
    p = os.path.join(ROOT, "status", "lockstep.json")
    doc = json.load(open(p)) if os.path.exists(p) else {"scenarios": {}}
    d = json.load(open(os.path.join(OUT, name + ".oracle.json")))
    turns = sorted({cp["turn"] for cp in d["checkpoints"]})
    import hashlib
    doc["scenarios"][name] = {"save_sha256": hashlib.sha256(open(path, "rb").read()).hexdigest(),
                              "checkpoints": total, "in_lockstep": same,
                              "turns": max(0, len(turns) - 1), "first_divergence": first}
    with open(p, "w") as f:
        json.dump(doc, f, indent=1, sort_keys=True)
        f.write("\n")


def unknown_bytes(save):
    """how much of each field the port's dump cannot speak for (pointers, layouts not shared)"""
    name, _path = scenario(save)
    E = recorder_module()
    nat = native_checkpoints(os.path.join(OUT, name + ".native", "native.cp"))
    if not nat:
        return {}
    return {f: m.count(0) for f, (_b, m) in native_blocks(nat[-1], E).items() if m.count(0)}


def main(argv):
    def opt(n, default=None):
        return argv[argv.index(n) + 1] if n in argv else default
    if argv[:1] == ["--oracle-run"]:
        return oracle_main(argv[1], int(argv[2]), argv[3], argv[4])
    turns = int(opt("--turns", "1"))
    if argv[:1] == ["oracle"]:
        oracle(argv[1], turns)
        return 0
    if argv[:1] == ["native"]:
        native(argv[1], turns, int(opt("--seconds", "1800")))
        print("bytes the port's dump cannot speak for:", unknown_bytes(argv[1]))
        return 0
    if argv[:1] == ["compare"] and "--full" in argv:
        k, _d = compare_full(argv[1], int(opt("--limit", "12")))
        return 0 if k is None else 1
    if argv[:1] == ["compare"]:
        same, total, first = compare(argv[1])
        if "--record" in argv:
            record(argv[1], same, total, first)
        return 0 if total and same == total else 1
    if argv[:1] == ["run"]:
        oracle(argv[1], turns)
        native(argv[1], turns, int(opt("--seconds", "1800")))
        same, total, first = compare(argv[1])
        if "--record" in argv:
            record(argv[1], same, total, first)
        return 0 if total and same == total else 1
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
