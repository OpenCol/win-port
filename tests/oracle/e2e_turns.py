# Carried by win-port: col-win-re's tools/e2e_turns.py as of 2026-10-03, which col-win-re has
# not committed yet (UPSTREAM.md). Unchanged below this header; tests/oracle.py runs it from
# the unpacked col-win-re tree at import/SOURCE, so REPO is that tree.
#!/usr/bin/env python3
"""End-to-end behaviour test: computer-only turns, the game state snapshotted per step.

Colonization plays itself. The Cheat menu's Human entry (CheatMenuHuman) answered
with "no nation" sets all four European nations to CONTROL_AI and switches on
g_autoplay, and from then on GameBrains_TurnControlLoop never waits for a person.
This tool does the same thing to a game started from the new-game dialogs, then
records the game's STATE at every step of every turn. The recording made from the
shipped binary is the golden. Any other build -- the relink, a module recompiled
with other options, eventually the port -- is held to it: different bytes, same
behaviour.

    tools/e2e_turns.py record                        # shipped -> tests/e2e/autoplay.json
    tools/e2e_turns.py check                         # analysis/rebuild vs the golden
    tools/e2e_turns.py check --exe X.EXE             # any index-compatible build
    tools/e2e_turns.py compare A.json B.json         # two recordings, offline
    tools/e2e_turns.py record --turns 3 --out /tmp/a.json --raw /tmp/a.raw.gz

WHERE IT RUNS. In the software Win16 machine (tools/re_agent/win16emu.py + winsim.py,
the same one tools/playtest_diff.py and tools/dyn_trace.py drive), not in Wine:
otvdm cannot be attached to (RE_TESTING.md §1), and this machine is deterministic by
construction -- no wall clock, no host RNG. Time is winsim's tick counter, which
only advances on API calls (an empty PeekMessage, GetTickCount), so the C runtime's
srand(time) and the game's ~20 srand(ticks) calls during setup get the same seeds
on every run -- PROVIDED input arrives at the same tick, which is why waits are in
ticks. The seeds are recorded ('seeds') and compared; --seed N adds N to each one,
which plays a different game just as reproducibly.

WHAT IS COMPARED, at each CHECKPOINT:
  state   the 57 blocks save_game (1008:a7f6) writes -- GameState, the nation
          texts, colonies/units/settlements by their live counts, nations, tribes,
          the region and tally tables, the four map planes behind their huge
          pointers, the trade routes, the seeds -- plus the C runtime's rand state
          (DGROUP:0x47c2, written by srand_rtl 1000:3d4e) and GameBrains. Hashed
          per RECORD (a unit, a colony, a map row), so a divergence names the
          record and the member.
  api     the Win16 API call names since the previous checkpoint, in order. Names
          only: argument bytes hold stack addresses and call sites hold code
          offsets, and both move when a build's code does.
  screen  the WinG framebuffer's pixels.

WHEN A CHECKPOINT IS TAKEN. At the first API call after (turn_counter,
GameBrains.step) changes. step is the turn controller's position: 0 the natives,
1-4 the four European viceroys, 5 the end of turn. Keying on the game's own state
at an API boundary, never on an instruction count, is what lets a build whose
instruction counts differ line up with the golden step for step.

WHEN INPUT IS GIVEN. The new-game script waits in TICKS and stops on the exact API
call where the clock reaches its target, then posts the key. A wait in
instructions (dyn_trace's `wait:`) would post a key at a build-dependent moment.

AUTOPLAY. Applied on every write to GameBrains.step while a game is in progress
(from the first one, during the new-game script, so it precedes the player's
first viceroy step): each
nation still CONTROL_HUMAN (0) becomes CONTROL_AI (1) and g_autoplay (DGROUP:0x88)
becomes 1 -- the stores CheatMenuHuman's all-AI branch makes, made before the
viceroy step reads them. If the game sits idle for NUDGE_TICKS without reaching a
new checkpoint (a modal message waiting for a key), Return is posted and recorded
as a 'nudge'; nudges are part of the recording and compared like everything else.

INDEX COMPATIBILITY. The emulator maps NE segment i to selector i*0x10000 and the
state addresses below are the shipped ones. A build must keep the segment table
(asserted, playtest_diff.assert_compatible) and the DATA layout of segment 20 and
DGROUP (not assertable from the header: a build that moved them fails at the
first checkpoint with every record different, which is what that looks like).

Exit codes: 0 match / recorded; 1 diverged; 2 could not run.
"""
import argparse, gzip, hashlib, json, os, struct, sys, time

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, 'tools'))
sys.path.insert(0, os.path.join(REPO, 'tools', 're_agent'))

SHIPPED = os.path.join(REPO, 'game', 'COLONIZE.EXE')
REBUILT = os.path.join(REPO, 'analysis', 'rebuild', 'COLONIZE.EXE')
GOLDEN = os.path.join(REPO, 'tests', 'e2e', 'autoplay.json')

SEG20, DGROUP, SEG27 = 20, 34, 27          # NE indices of 1098:, 1108:, 10d0:
GAME = 0x7782                               # SEG20: GameState, 0x8e bytes
BRAINS = 0x40d6                             # SEG20: GameBrains {f0, step, phase, ...}
NATION_TEXT = 0x7810                        # SEG20: NationText[4], 0x34 each
CONTROL = 0x31                              # NationText.control: 0 human, 1 AI, 2 out
AUTOPLAY = 0x88                             # DGROUP: g_autoplay
SRAND = '1000:3d4e'                         # srand_rtl
QUIT_FLAG = 0x4b04                          # DGROUP: g_quit_flag
# The runtime's ways out. Entering one ends the game; where from is the evidence.
EXITS = {'1000:51b5': 'exit_rtl', '1000:51ca': '_exit_rtl', '1000:50f0': 'abort_rtl',
         '1000:00dd': '_terminate_rtl'}
# A C++ throw, caught on entry while the stack still says where it came from.
THROWS = {'1090:175f': '_ThrowException', '1090:1975': 'RaiseException'}
NUDGE_TICKS = 200_000                       # idle this long with no checkpoint -> Return
STALL_INSTR = 300_000_000                   # this long with no API call -> stop, report

# The new-game script, from dyn_trace.py's measured trunk, with every wait in winsim
# ticks (milliseconds of the emulator's own clock) instead of instructions.
SETUP = [
    ('boot', 'wait:112000'),                # title screen's message loop (170 M instr)
    ('title', 'key:Return,wait:30000'),     # the intro ends
    ('begin_menu', 'key:Return,wait:30000'),
    ('new_game', 'key:Return,wait:30000'),  # start_new_game -> SetDifficulty
    ('difficulty', 'key:Space,wait:30000'),  # Space = the focused ~Ok button
    ('nation', 'key:Space,wait:30000'),     # SetNations Ok -> leader-name box
    ('leader', 'key:Return,wait:30000'),
    ('intro_page', 'key:Return,wait:30000'),
    ('king', 'key:Return,wait:30000'),
    # World generation. dyn_trace's script sends nine keys here to dismiss its
    # messages; two are enough, because the turn loop starts during the second
    # and from then on autoplay plays the player's nation too.
    ('mapgen0', 'key:Space,wait:30000'),
    ('mapgen1', 'key:Return,wait:30000'),
]

# (name, segment, offset, size) in save_game's write order. A size that is a tuple
# (count_field_offset, stride) is the live count in GameState times the stride.
STATE = [
    ('map_size', DGROUP, 0x4c40, 4),
    ('game', SEG20, GAME, 0x8e),
    ('nation_text', SEG20, NATION_TEXT, 0xd0),
    ('rec6_78e0', SEG20, 0x78e0, 0x18),
    ('colonies', SEG20, 0x7c28, (30, 0xca)),
    ('units', SEG20, 0xa208, (28, 0x1c)),
    ('nations', SEG20, 0xc2f4, 0x4f0),
    ('settlements', SEG20, 0xcb9a, (26, 0x12)),
    ('tribes', SEG20, 0xd182, 0x270),
    ('leader_traits', SEG20, 0x7c18, 0xc),
    ('nation_tallies', DGROUP, 0x4b44, 0x2c),      # 0x4b44..0x4b6c, eleven 4-byte rows
    ('military_power', SEG20, 0xd9d6, 8),
    ('unit_tally', SEG20, 0xd9de, 0x4c),
    ('avg_colony_size', SEG20, 0xda48, 8),
    ('region_tables', SEG20, 0xda50, 0x1b0),       # farmland .. region_flags
    ('tribe_tables', SEG20, 0xdc00, 0x98),         # strength, pop, count, region
    ('show_icon_counts', DGROUP, 0xc0e, 1),
    ('cursor', DGROUP, 0x4b30, 4),
    ('map_view', DGROUP, 0x1c92, 4),               # map centre x, y
    ('zoom', DGROUP, 0x1c9a, 2),
    ('coarse_grids', SEG27, 0xae, 0x21c),          # land, sea
    ('region_size', SEG27, 0x8c, 0x20),
    ('seed_base', DGROUP, 0x4bc2, 4),
    ('scenery_seed', DGROUP, 0x1ca6, 2),
    ('routes', SEG20, 0xc804, 0x378),
    # not saved, but the state the next turn is computed from
    ('game_seeds', DGROUP, 0x4b86, 6),             # g_random/event/music_seed
    ('rtl_rand', DGROUP, 0x47c2, 4),               # rand()'s 32-bit state
    ('brains', SEG20, BRAINS, 12),
    ('autoplay', DGROUP, AUTOPLAY, 2),
]
PLANES = [(f'plane{i}', 0x1c72 + 4 * i) for i in range(4)]   # huge far pointers
MAP_BYTES = 0x1c96                                            # DGROUP long

# Which record a field holds, for naming a divergence: (model.json record, stride).
RECORDS = {'game': ('GameState', 0x8e), 'nation_text': ('NationText', 0x34),
           'colonies': ('Colony', 0xca), 'units': ('Unit', 0x1c),
           'nations': ('Nation', 0x13c), 'settlements': ('Settlement', 0x12),
           'tribes': ('Tribe', 0x4e), 'brains': ('GameBrains', 12)}


def reexec_under_venv():
    import dyn_trace
    dyn_trace.reexec_under_venv()


def h8(b):
    return hashlib.sha1(b).hexdigest()[:12]


# --------------------------------------------------------------------- recording
class Recorder:
    def __init__(self, exe, seed=None, turns=5, max_instr=6_000_000_000,
                 progress=True, raw_path=None):
        import dyn_trace as DT
        import playtest_diff as PD
        import win16emu as W
        from unicorn import UC_HOOK_MEM_WRITE
        self.exe = os.path.abspath(exe)
        ne, rel = PD.tables(self.exe)
        if self.exe != os.path.abspath(SHIPPED):
            bad = PD.assert_compatible(ne)
            if bad:
                raise SystemExit('e2e_turns: not index-compatible: ' + '; '.join(bad))
        DT.SETTLE = 0                      # input settles in ticks here, not instructions
        self.DT = DT
        self.t = DT.Tracer({}, image=W.Image(exe=self.exe, ne_json=ne, reloc_json=rel))
        self.g, self.m, self.sim = self.t.g, self.t.g.m, self.t.g.sim
        self.uc = self.m.uc
        self.seed, self.turns, self.max_instr = seed, turns, max_instr
        self.progress, self.raw_path = progress, raw_path
        self.raw = [] if raw_path else None
        self.checkpoints, self.seeds, self.nudges, self.notes = [], [], [], []
        self.events = []                   # game over, quit, exit: where from
        self.recent_api = []               # the last 32 (name, call site)
        self._names = None
        self.api_n, self.api_hash = 0, hashlib.sha1()
        self.key = None
        self.stop_ticks = None             # stop at the API call that reaches this
        self.halt = False                  # stop now (turn limit reached)
        self.autoplay = False
        self.start_turn = None
        self.last_progress = 0
        self.phase = 'boot'
        self.base = {i: self.m.base(i) for i in (SEG20, DGROUP, SEG27)}
        self.m.on_call(SRAND, self._on_srand, 'srand_rtl')
        lin = self.base[SEG20] + BRAINS + 2
        self.uc.hook_add(UC_HOOK_MEM_WRITE, self._on_step_write, None, lin, lin + 1)
        for addr, name in EXITS.items():
            self.m.on_call(addr, lambda m, raw, n=name: self.exiting(n), name)
        for addr, name in THROWS.items():
            self.m.on_call(addr, lambda m, raw, n=name: self.event(n), name)
        q = self.base[DGROUP] + QUIT_FLAG
        self.uc.hook_add(UC_HOOK_MEM_WRITE, self._on_quit_write, None, q, q + 1)
        gip = self.base[SEG20] + GAME + 66
        self.uc.hook_add(UC_HOOK_MEM_WRITE, self._on_progress_write, None, gip, gip + 1)
        self.blocks_ring = None
        if os.environ.get('E2E_BLOCKS'):
            # Diagnostics: the last 256 basic blocks executed, for a CPU error.
            from unicorn import UC_HOOK_BLOCK
            from collections import deque
            ring = self.blocks_ring = deque(maxlen=256)
            self.uc.hook_add(UC_HOOK_BLOCK, lambda uc, a, sz, ud: ring.append(a))
        self.handles_by_api, self.cur_api = {}, None
        real_handle = self.sim.handle

        def counted_handle():
            self.handles_by_api[self.cur_api] = self.handles_by_api.get(self.cur_api, 0) + 1
            return real_handle()
        self.sim.handle = counted_handle
        for lin, (mod, name) in self.sim._by_trap.items():
            st = self.m.stubs[lin]
            st['handler'] = self._wrap(st['handler'], f'{mod}.{name}')

    # -- memory
    def rd(self, idx, off, n):
        return bytes(self.uc.mem_read(self.base[idx] + off, n))

    def w16(self, idx, off):
        return struct.unpack('<H', self.rd(idx, off, 2))[0]

    def game_key(self):
        g = self.rd(SEG20, GAME, 0x16)
        turn, = struct.unpack_from('<h', g, 14)
        step, = struct.unpack('<h', self.rd(SEG20, BRAINS + 2, 2))
        return turn, step

    # -- hooks
    def _on_srand(self, machine, raw):
        v = struct.unpack_from('<H', raw)[0]
        rec = {'api_calls': self.api_n, 'phase': self.phase, 'seed': v}
        if self.seed:
            # The game reseeds from the tick clock ~20 times while it sets up a
            # game; a constant would make every one of them the same. An offset
            # picks another game and keeps them distinct.
            from unicorn.x86_const import UC_X86_REG_SS, UC_X86_REG_SP
            sp = self.uc.reg_read(UC_X86_REG_SP)
            st = machine.seg_base(self.uc.reg_read(UC_X86_REG_SS)) + sp
            nv = (v + self.seed) & 0x7FFF
            self.uc.mem_write(st + 4, struct.pack('<H', nv))
            rec['seed'] = nv
            rec['unforced'] = v
        self.seeds.append(rec)

    def _on_quit_write(self, uc, access, address, size, value, ud):
        if value & 0xFFFF:
            self.event(f'g_quit_flag = {value & 0xFFFF}')

    def _on_progress_write(self, uc, access, address, size, value, ud):
        old, = struct.unpack('<h', bytes(uc.mem_read(address, 2)))
        if old and not value & 0xFFFF:
            self.event('game_in_progress = 0')

    def exiting(self, what):
        """The program is ending (WinMain returned, exit, abort): the run is over.

        Stopped here rather than left to run: winsim fails INT 21h/4Ch, so
        _terminate falls through into exit again and the exit procedures call the
        API forever -- which the no-API stall check cannot see."""
        if self.t.stopped:
            return
        self.event(what)
        self.t.stopped = {'error': f'the program exited ({what})',
                          'checkpoint': len(self.checkpoints) - 1, 'key': self.key}
        self.uc.emu_stop()

    def event(self, what):
        self.events.append({'event': what, 'checkpoint': len(self.checkpoints) - 1,
                            'key': self.key, 'api_calls': self.api_n,
                            'callstack': [self.fn(f.get('pc'))
                                          for f in self.m.callstack(16)],
                            'recent_api': list(self.recent_api[-12:])})
        if self.progress:
            print(f'    EVENT {what} at checkpoint {len(self.checkpoints) - 1} '
                  f'{self.key}: {" <- ".join(str(x) for x in self.events[-1]["callstack"][:8])}',
                  flush=True)

    def fn(self, pc):
        """'1008:2a10' -> '1008:2a10 GameBrains_DoEndofTurn+0x5c'."""
        if not isinstance(pc, str) or ':' not in pc:
            return pc
        if self._names is None:
            import dyn_trace as DT
            self._names = {}
            for a, n in DT.census().items():
                sg, off = a.split(':')
                self._names.setdefault(sg.lower(), []).append((int(off, 16), n or a))
            for v in self._names.values():
                v.sort()
        sg, off = pc.split(':')
        off, best = int(off, 16), None
        for o, n in self._names.get(sg.lower(), []):
            if o > off:
                break
            best = (o, n)
        return f'{pc} {best[1]}+0x{off - best[0]:x}' if best else pc

    def _on_step_write(self, uc, access, address, size, value, ud):
        # Armed from boot: by the end of the new-game script the player's first
        # viceroy step has already run and the loop is parked on awaiting_human,
        # so the stores must land on the step write BEFORE it (0 -> 1, after the
        # natives). Only once a game is in progress, as the cheat menu requires.
        if not struct.unpack('<h', bytes(uc.mem_read(self.base[SEG20] + GAME + 66, 2)))[0]:
            return
        for i in range(4):
            a = self.base[SEG20] + NATION_TEXT + 0x34 * i + CONTROL
            if uc.mem_read(a, 1)[0] == 0:
                uc.mem_write(a, b'\x01')
        uc.mem_write(self.base[DGROUP] + AUTOPLAY, b'\x01\x00')

    def _wrap(self, orig, name):
        def handler(machine, stub, raw, ret, st):
            self.api_n += 1
            k = self.game_key()
            if k != self.key:
                self.checkpoint(k)
            self.api_hash.update(name.encode() + b'\n')
            self.cur_api = name
            self.recent_api.append(f'{name} from {ret[0]:04x}:{ret[1]:04x}')
            if len(self.recent_api) > 64:
                del self.recent_api[:32]
            r = orig(machine, stub, raw, ret, st)
            if self.stop_ticks is not None and self.sim.ticks >= self.stop_ticks:
                self.stop_ticks = None
                self.uc.emu_stop()
            elif self.halt:
                self.uc.emu_stop()
            elif self.autoplay and \
                    self.sim.ticks - self.last_progress > NUDGE_TICKS:
                self.nudge()
            return r
        return handler

    def nudge(self):
        from winsim import WM_KEYDOWN, WM_CHAR, WM_KEYUP
        self.last_progress = self.sim.ticks
        ok = self.t.post((WM_KEYDOWN, 0x0D, 1), (WM_CHAR, 0x0D, 1),
                         (WM_KEYUP, 0x0D, 0xC0000001), keyboard=True)
        self.nudges.append({'checkpoint': len(self.checkpoints), 'key': self.key,
                            'api_calls': self.api_n, 'delivered': ok})

    # -- the snapshot
    def state(self):
        """{field: bytes} -- the save image plus the generators' state."""
        game = self.rd(SEG20, GAME, 0x8e)
        out = {}
        for name, idx, off, size in STATE:
            if isinstance(size, tuple):
                cnt, = struct.unpack_from('<h', game, size[0])
                size = min(max(0, cnt) * size[1], 0x10000 - off)
            out[name] = self.rd(idx, off, size) if size else b''
        n, = struct.unpack('<I', self.rd(DGROUP, MAP_BYTES, 4))
        for name, ptr in PLANES:
            off, sel = struct.unpack('<HH', self.rd(DGROUP, ptr, 4))
            try:
                out[name] = bytes(self.uc.mem_read(self.m.seg_base(sel) + off, n)) \
                    if sel and 0 < n <= 0x40000 else b''
            except Exception:
                out[name] = b''
        return out

    def screen(self):
        try:
            d, px = self.g.pixels()
        except Exception:
            return None
        return h8(px) if px else None

    def checkpoint(self, k):
        self.key = k
        self.last_progress = self.sim.ticks
        st = self.state()
        mw = struct.unpack_from('<h', st['map_size'], 0)[0] if st['map_size'] else 0
        cp = {'i': len(self.checkpoints), 'turn': k[0], 'step': k[1],
              'phase': self.phase, 'autoplay': self.autoplay,
              'api_calls': self.api_n, 'api_hash': self.api_hash.hexdigest()[:12],
              'screen': self.screen(),
              'hash': {f: hashes(f, b, mw) for f, b in st.items()},
              'summary': summary(st),
              # informational only: never compared
              'instr': self.m.log['instr'], 'ticks': self.sim.ticks}
        self.api_hash = hashlib.sha1()
        self.checkpoints.append(cp)
        if self.raw is not None:
            self.raw.append({f: b.hex() for f, b in st.items()})
        if self.autoplay:
            if self.start_turn is None:
                self.start_turn = k[0]
            if k[0] - self.start_turn >= self.turns:
                self.halt = True
        if self.progress and (self.autoplay or self.phase != 'boot'):
            s = cp['summary']
            print(f'    cp{cp["i"]:<4} turn {k[0]:>3} step {k[1]:>2}  {s["year"]}'
                  f'{"ab"[s["season"] & 1] if s["year"] >= 1600 else " "} '
                  f'units {s["units"]:>3} colonies {s["colonies"]:>2} '
                  f'gold {s["gold"]}  rand {s["rtl_rand"]}  '
                  f'{cp["instr"]:>13,} instr', flush=True)

    # -- running
    def run(self, n):
        """Run up to n instructions; returns early when a hook stopped the CPU."""
        g, t = self.g, self.t
        if not t.started:
            r = g.start(min(n, 4_000_000))
            t.started = True
            if r.get('error'):
                t.stopped = {'error': r['error'], 'pc': r['stopped_at']}
            return
        t._resume = False
        r = g._run(n)
        if r and r.get('error'):
            t.stopped = {'error': r['error'], 'pc': self.fn(r['pc']),
                         'checkpoint': len(self.checkpoints) - 1, 'key': self.key,
                         'callstack': [self.fn(f.get('pc'))
                                       for f in self.m.callstack(16)],
                         'recent_api': self.recent_api[-24:],
                         'cpu': self.cpu_dump()}
        elif r and r['ran'] == 0:
            # A hook's emu_stop always lets at least the stub's own instruction
            # count; three empty runs in a row is a CPU that will not move.
            self.idle_runs = getattr(self, 'idle_runs', 0) + 1
            if self.idle_runs >= 3:
                t.stopped = t.diagnose(r)
        else:
            self.idle_runs = 0

    def cpu_dump(self):
        import unicorn.x86_const as C
        uc, m = self.uc, self.m
        R = {r: uc.reg_read(getattr(C, 'UC_X86_REG_' + r)) for r in
             ('AX', 'BX', 'CX', 'DX', 'SI', 'DI', 'BP', 'SP', 'CS', 'IP', 'DS', 'ES', 'SS',
              'FS', 'GS', 'ESI', 'EDI')}
        out = {'regs': {k: f'{v:04x}' for k, v in R.items()}}
        # which allocation each segment register points into, and its history
        out['segments'] = {r: {k: v for k, v in m.blocks[R[r]].items() if k != 'base'}
                           for r in ('DS', 'ES', 'FS', 'GS') if R[r] in m.blocks}
        try:
            lin = m.seg_base(R['CS']) + R['IP']
            out['code_live'] = bytes(uc.mem_read(lin - 8, 16)).hex()
            idx = lin >> 16
            if idx in m.image.segs:
                out['code_file'] = bytes(m.image.seg_bytes(idx)[(lin & 0xFFFF) - 8:
                                                              (lin & 0xFFFF) + 8]).hex()
            st = m.seg_base(R['SS']) + R['SP']
            out['stack'] = bytes(uc.mem_read(st, 32)).hex()
        except Exception as e:
            out['dump_error'] = str(e)
        if self.blocks_ring is not None:
            out['last_blocks'] = [self.fn(m.where(a).get('addr') or f'{a:x}')
                                  for a in list(self.blocks_ring)[-48:]]
        return out

    def stall_report(self, samples):
        fn = self.fn
        from collections import Counter
        return {'error': f'no API call for {STALL_INSTR:,} instructions',
                'checkpoint': len(self.checkpoints) - 1, 'key': self.key,
                'pc_samples': [f'{fn(p)} x{c}' for p, c in Counter(samples).most_common(12)],
                'callstack': [fn(f.get('pc')) for f in self.m.callstack(16)],
                'divide_patches': self.t.divide_patches[-5:],
                'recent_api': self.recent_api[-16:],
                'cpu': self.cpu_dump()}

    def until_ticks(self, ms, cap=1_500_000_000):
        target = self.sim.ticks + ms
        self.stop_ticks = target
        i0 = self.m.log['instr']
        while self.sim.ticks < target and not self.t.stopped and not self.g.finished:
            if self.m.log['instr'] - i0 > cap:
                self.notes.append(f'{self.phase}: wait:{ms} hit the {cap:,} '
                                  f'instruction cap at tick {self.sim.ticks}')
                break
            self.run(2_000_000)
        self.stop_ticks = None

    def step(self, kind, arg):
        if kind == 'wait':
            self.until_ticks(arg)
        elif kind == 'run':
            self.run(arg)
        else:
            if not self.t.started:
                self.run(4_000_000)
            note = self.t.step(kind, arg)     # posts the input; SETTLE is 0
            if note:
                self.notes.append(f'{self.phase}: {note}')

    def record(self):
        t0 = time.time()
        for name, spec in SETUP:
            self.phase = name
            for kind, arg in self.DT.parse_steps(spec):
                if self.t.stopped or self.g.finished:
                    break
                self.step(kind, arg)
            if self.progress:
                print(f'  {name:12} {self.m.log["instr"]:>13,} instr '
                      f'{self.sim.ticks:>9,} ticks {time.time() - t0:7.0f}s '
                      f'turn/step {self.game_key()}', flush=True)
        self.phase = 'autoplay'
        self.autoplay = True
        self.last_progress = self.sim.ticks
        self.t.target = None
        i0 = self.m.log['instr']
        last_api, since, samples = self.api_n, self.m.log['instr'], []
        while not self.halt and not self.t.stopped and not self.g.finished:
            if self.m.log['instr'] - i0 > self.max_instr:
                self.notes.append(f'autoplay hit the {self.max_instr:,} instruction cap')
                break
            self.run(5_000_000)
            # A game that makes no API call for this long is in a loop of its own:
            # stop with where it is, instead of spinning to the instruction cap.
            if self.api_n != last_api:
                last_api, since, samples = self.api_n, self.m.log['instr'], []
                continue
            samples.append(self.m._pc())
            if self.m.log['instr'] - since > STALL_INSTR:
                self.t.stopped = self.stall_report(samples)
                break
        doc = {
            'tool': 'tools/e2e_turns.py',
            'exe': os.path.relpath(self.exe, REPO),
            'exe_sha256': hashlib.sha256(open(self.exe, 'rb').read()).hexdigest(),
            'setup': SETUP, 'turns': self.turns, 'forced_seed': self.seed,
            'seeds': self.seeds, 'nudges': self.nudges, 'events': self.events,
            'start_turn': self.start_turn,
            'end': {'stopped': self.t.stopped, 'program_returned': self.g.finished,
                    'turn_limit_reached': self.halt,
                    'divide_patches': len(self.t.divide_patches),
                    'selectors_recycled': self.m.recycled,
                    'handle_wraps': getattr(self.sim, 'handle_wraps', 0),
                    'handles_by_api': dict(sorted(self.handles_by_api.items(),
                                                  key=lambda kv: -kv[1])[:30]),
                    'defaults_used': dict(sorted(self.sim.defaults_used.items(),
                                                 key=lambda kv: -kv[1])[:40]),
                    # winsim swallows an exception in an API handler and returns a
                    # default; that is how selector exhaustion first hid
                    'handler_errors': self.sim.errors[:20]},
            'notes': self.notes,
            'budget': {'instructions': self.m.log['instr'],
                       'seconds': round(time.time() - t0, 1)},
            'checkpoints': self.checkpoints,
        }
        if self.raw is not None:
            with gzip.open(self.raw_path, 'wt') as f:
                json.dump(self.raw, f)
        return doc


def hashes(field, b, map_w):
    """A field's hash: per record for the record tables, per row for a map plane."""
    if field in RECORDS and field != 'game':
        stride = RECORDS[field][1]
        return [h8(b[i:i + stride]) for i in range(0, len(b), stride)]
    if field.startswith('plane') and map_w > 0:
        return [h8(b[i:i + map_w]) for i in range(0, len(b), map_w)]
    return h8(b)


def summary(st):
    """The few numbers a person reads first. Compared only through the hashes."""
    g = st['game']
    year, season, turn = struct.unpack_from('<hhh', g, 10)
    tn, = struct.unpack_from('<h', g, 20)
    units, colonies = struct.unpack_from('<hh', g, 28)
    gold = [struct.unpack_from('<i', st['nations'], 0x13c * i + 42)[0]
            for i in range(4)] if len(st['nations']) >= 0x4f0 else []
    ctrl = [st['nation_text'][0x34 * i + CONTROL] for i in range(4)] \
        if len(st['nation_text']) >= 0xd0 else []
    return {'year': year, 'season': season, 'turn': turn, 'turn_nation': tn,
            'units': units, 'colonies': colonies, 'gold': gold, 'control': ctrl,
            'rtl_rand': st['rtl_rand'].hex()}


# --------------------------------------------------------------------- comparing
_members = None


def member_at(record, off):
    global _members
    if _members is None:
        try:
            _members = json.load(open(os.path.join(REPO, 'analysis', 'model.json')))['records']
        except (OSError, ValueError):
            _members = {}
    for mm in _members.get(record, {}).get('members', []):
        if mm['offset'] <= off < mm['offset'] + mm['size']:
            return mm['name']
    return f'+0x{off:x}'


def compare(a, b, limit=12):
    """[str] -- empty when the two recordings agree checkpoint for checkpoint."""
    out = []
    if a.get('seeds') and [s['seed'] for s in a['seeds']] != \
            [s['seed'] for s in b.get('seeds', [])]:
        out.append(f'seeds: {[s["seed"] for s in a["seeds"]]} != '
                   f'{[s["seed"] for s in b.get("seeds", [])]}')
    ca, cb = a['checkpoints'], b['checkpoints']
    for i in range(min(len(ca), len(cb))):
        x, y = ca[i], cb[i]
        where = f'checkpoint {i} (turn {x["turn"]} step {x["step"]}, {x["phase"]})'
        if (x['turn'], x['step']) != (y['turn'], y['step']):
            out.append(f'{where}: the other run is at turn {y["turn"]} step {y["step"]}')
            break
        diffs = []
        for f in x['hash']:
            hx, hy = x['hash'][f], y['hash'].get(f)
            if hx == hy:
                continue
            if isinstance(hx, list) and isinstance(hy, list):
                rows = [j for j in range(max(len(hx), len(hy)))
                        if j >= len(hx) or j >= len(hy) or hx[j] != hy[j]]
                unit = 'row' if f.startswith('plane') else 'record'
                diffs.append(f'{f}: {len(hx)} vs {len(hy)} {unit}s, differing '
                             f'{unit}s {rows[:10]}{" ..." if len(rows) > 10 else ""}')
            else:
                diffs.append(f'{f}')
        if x['api_hash'] != y['api_hash'] or x['api_calls'] - (ca[i - 1]['api_calls'] if i else 0) \
                != y['api_calls'] - (cb[i - 1]['api_calls'] if i else 0):
            diffs.append('api: the call sequence since the previous checkpoint differs')
        if x.get('screen') != y.get('screen'):
            diffs.append('screen: the framebuffer differs')
        if diffs:
            out.append(f'{where}: FIRST DIVERGENCE')
            out += ['    ' + d for d in diffs[:limit]]
            sx, sy = x['summary'], y['summary']
            for k in sx:
                if sx[k] != sy.get(k):
                    out.append(f'    summary.{k}: {sx[k]} != {sy.get(k)}')
            return out
    if len(ca) != len(cb):
        out.append(f'checkpoint count {len(ca)} != {len(cb)} '
                   f'(the shorter run ended at turn {(ca if len(ca) < len(cb) else cb)[-1]["turn"]})'
                   if ca and cb else f'checkpoint count {len(ca)} != {len(cb)}')
    xa, xb = a.get('end', {}), b.get('end', {})
    for k in ('stopped', 'selectors_recycled', 'handle_wraps', 'divide_patches'):
        va, vb = xa.get(k), xb.get(k)
        if k == 'stopped':
            va, vb = (va or {}).get('error'), (vb or {}).get('error')
        if va != vb:
            out.append(f'end.{k}: {va} != {vb}')
    if len(xa.get('handler_errors', [])) != len(xb.get('handler_errors', [])):
        out.append(f'end.handler_errors: {len(xa.get("handler_errors", []))} != '
                   f'{len(xb.get("handler_errors", []))}')
    ea = [(e['event'], e['checkpoint']) for e in a.get('events', [])]
    eb = [(e['event'], e['checkpoint']) for e in b.get('events', [])]
    if ea != eb:
        out.append(f'events (quit, game over, exit) {ea} != {eb}')
    na, nb = a.get('nudges', []), b.get('nudges', [])
    if [n['checkpoint'] for n in na] != [n['checkpoint'] for n in nb]:
        out.append(f'nudges at checkpoints {[n["checkpoint"] for n in na]} != '
                   f'{[n["checkpoint"] for n in nb]}')
    return out


def explain(raw_a, raw_b, i, field, limit=8):
    """Member-level detail for one field at checkpoint i, from two --raw dumps."""
    a, b = bytes.fromhex(raw_a[i][field]), bytes.fromhex(raw_b[i][field])
    rec, stride = RECORDS.get(field, (None, 0))
    out = []
    for off in range(max(len(a), len(b))):
        va = a[off] if off < len(a) else None
        vb = b[off] if off < len(b) else None
        if va != vb:
            name = f'{field}+0x{off:x}'
            if rec:
                name = f'{field}[{off // stride}].{member_at(rec, off % stride)}'
            out.append(f'{name}: {va} != {vb}')
            if len(out) >= limit:
                break
    return out


# --------------------------------------------------------------------------- cli
def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    sub = ap.add_subparsers(dest='cmd', required=True)
    for name in ('record', 'check'):
        p = sub.add_parser(name)
        p.add_argument('--exe', default=SHIPPED if name == 'record' else REBUILT)
        p.add_argument('--golden', default=GOLDEN)
        p.add_argument('--out', help='where the recording goes '
                       '(record: the golden; check: not kept unless given)')
        p.add_argument('--raw', help='also write the raw state bytes here (.json.gz); '
                       'derived game bytes, so never under a tracked path')
        p.add_argument('--turns', type=int, help='autoplay turns (record default 5; '
                       'check default: the golden\'s)')
        p.add_argument('--seed', type=int, help='add this to every srand() seed: '
                       'another game, as reproducible as the default one')
        p.add_argument('--max-instr', type=int, default=6_000_000_000)
        p.add_argument('-q', '--quiet', action='store_true')
    p = sub.add_parser('compare')
    p.add_argument('a')
    p.add_argument('b')
    p.add_argument('--raw', nargs=2, metavar=('RAW_A', 'RAW_B'))
    args = ap.parse_args()

    if args.cmd == 'compare':
        a, b = json.load(open(args.a)), json.load(open(args.b))
        d = compare(a, b)
        print('\n'.join(d) if d else f'MATCH: {len(a["checkpoints"])} checkpoints')
        if d and args.raw:
            ra, rb = (json.load(gzip.open(p, 'rt')) for p in args.raw)
            first = next(int(l.split()[1]) for l in d if l.startswith('checkpoint '))
            for f in ra[first]:
                if ra[first][f] != rb[first].get(f):
                    print('\n'.join('  ' + l for l in explain(ra, rb, first, f)))
        return 1 if d else 0

    reexec_under_venv()
    golden = None
    if args.cmd == 'check':
        if not os.path.exists(args.golden):
            print(f'e2e_turns: no golden at {args.golden}; run `record` first')
            return 2
        golden = json.load(open(args.golden))
        turns = args.turns or golden['turns']
        seed = args.seed if args.seed is not None else golden.get('forced_seed')
    else:
        turns, seed = args.turns or 5, args.seed
    if not os.path.exists(args.exe):
        print(f'e2e_turns: {args.exe} does not exist')
        return 2
    print(f'e2e_turns {args.cmd}: {os.path.relpath(args.exe, REPO)}, {turns} turns'
          f'{", seed %d" % seed if seed is not None else ""}', flush=True)
    rec = Recorder(args.exe, seed=seed, turns=turns, max_instr=args.max_instr,
                   progress=not args.quiet, raw_path=args.raw)
    doc = rec.record()
    out = args.out or (args.golden if args.cmd == 'record' else None)
    if out:
        os.makedirs(os.path.dirname(os.path.abspath(out)), exist_ok=True)
        with open(out, 'w') as f:
            json.dump(doc, f, indent=1)
            f.write('\n')
    e = doc['end']
    print(f'{len(doc["checkpoints"])} checkpoints, {len(doc["nudges"])} nudges, '
          f'seeds {[s["seed"] for s in doc["seeds"]]}, '
          f'{doc["budget"]["instructions"]:,} instructions in {doc["budget"]["seconds"]}s'
          f'{"; STOPPED " + json.dumps(e["stopped"]) if e["stopped"] else ""}'
          f'{"; notes: " + "; ".join(doc["notes"]) if doc["notes"] else ""}')
    if args.cmd == 'record':
        if not e['turn_limit_reached']:
            print('e2e_turns: the turn limit was NOT reached; the recording is short')
            return 2
        return 0
    d = compare(golden, doc)
    if d:
        print('DIVERGED from ' + os.path.relpath(args.golden, REPO))
        print('\n'.join(d))
        return 1
    print(f'MATCH: {len(doc["checkpoints"])} checkpoints agree with '
          f'{os.path.relpath(args.golden, REPO)}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
