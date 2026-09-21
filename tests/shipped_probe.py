#!/usr/bin/env python3
"""Run the SHIPPED function and report what it returned and what memory then held.

Runs under the byte-exact repo's unicorn interpreter, because that repo owns the emulator
and this one must not fork it. It is called by tests/differential.py, never by hand.

It reports FINAL VALUES at every address the function wrote, not the write log itself.
The host side cannot produce a write log -- it has no MMU and only diffs its slabs -- and
a write of a value that was already there is invisible to a diff. Comparing final state at
the addresses the shipped code touched is a question both sides can answer.
"""
import json, os, sys

CASE = json.load(open(sys.argv[1]))
SRC = sys.argv[2]
DUMP = sys.argv[3] if len(sys.argv) > 3 else None
sys.path.insert(0, os.path.join(SRC, 'tools', 're_agent'))
sys.path.insert(0, os.path.join(SRC, 'tools'))
import probe  # noqa: E402


def seg_of(spec):
    """'SEG20:0x1000' or 'DGROUP:0x4b9e' -> (segment index, offset)."""
    where, _, off = spec.partition(':')
    seg = 34 if where == 'DGROUP' else int(where[3:])
    return seg, int(off, 0)


def selector(seg):
    return 0x1000 + (seg - 1) * 8


def main():
    m, meta = probe.build(CASE.get('state', 'save'), CASE.get('stubs', []))
    for spec, hexbytes in CASE.get('stage', {}).items():
        seg, off = seg_of(spec)
        m.write(seg, off, bytes.fromhex(hexbytes))
    for at, points_to in CASE.get('pointers', {}).items():
        seg, off = seg_of(at)
        tseg, toff = seg_of(points_to)
        m.write(seg, off, bytes([toff & 0xFF, toff >> 8,
                                 selector(tseg) & 0xFF, selector(tseg) >> 8]))
    argsets = probe.parse_sweep(CASE.get('sweep', []), CASE.get('call_args', []))

    # One machine serves every argument set, so a function that WRITES would otherwise see
    # what the previous call left -- and the host resets between calls, so the two would
    # disagree for a reason that is the harness's fault, not the port's. TextColor returns
    # the previous colour; without this it returned a different one every time and every
    # comparison after the first was meaningless. Capture the data segments once, after
    # staging, and put them back before each call.
    pristine = {}
    for idx, seg in m.image.segs.items():
        if m.image.is_code(idx):
            continue
        n = min(seg.get('min_alloc') or seg['length'] or 0x10000, 0x10000)
        pristine[idx] = (m.read(idx, 0, n), n)

    def restore():
        for idx, (data, n) in pristine.items():
            m.write(idx, 0, data)

    def addrs(kind):
        """Every byte address the last call READ or WROTE, outside the stack."""
        out = set()
        for w in m.log[kind]:
            if w.get('space') in ('stack', 'scratch') or not w.get('addr'):
                continue
            sel, _, off = w['addr'].partition(':')
            seg = (int(sel, 16) - 0x1000) // 8 + 1
            for k in range(w.get('size', 1)):
                out.add((seg, int(off, 16) + k))
        return out

    def written():
        return addrs('writes')

    if DUMP:
        # The host's idea of "the shipped state" is the raw segment image; the emulator's
        # is that image AFTER its startup has run, and the startup writes into DGROUP --
        # a selector at 0x2ad8, and more. Any function that reads those bytes sees
        # different memory on the two sides for a reason that is nothing to do with the
        # port: append_period_spaces is a strcat, and one byte's difference in where the
        # first NUL falls moves the whole copy. So the emulator's state is handed to the
        # host rather than approximated.
        import struct as _s
        with open(DUMP, 'wb') as f:
            for idx, (data, n) in sorted(pristine.items()):
                f.write(_s.pack('<HI', idx, len(data)) + bytes(data))

    out = []
    for args in argsets:
        # TWO calls per argument set, and the second one is the one that counts.
        #
        # The host cannot produce a write LOG -- it diffs memory -- so a write that stores
        # a value already present is invisible there. Seven functions here do nothing but
        # that (clearing a buffer that ships zeroed), and for them a port that did nothing
        # at all passed. So: call once to learn which bytes the shipped code writes, put
        # the memory back, POISON exactly those bytes to the complement of what they held,
        # and call again. Now every write changes something, on both sides -- the host is
        # given the same poison.
        restore()
        probe.observe(m, CASE['shipped'], args, CASE.get('max_instr', 200000))
        first = written()
        # Poison with the complement of what the function WROTE, not of what was there
        # before. rec6_0676_reset writes 0xff over bytes that ship as 0x00, so poisoning
        # with the complement of the pristine byte put 0xff there -- exactly the value the
        # function goes on to write -- and a port that did nothing still matched.
        wrote = {(seg, off): m.read(seg, off, 1)[0] for seg, off in first}
        # Poison every byte the function wrote. Excluding the ones it also READ was too
        # cautious: unit_clear_flags_high does `flags &= 0xf` on a byte whose high nibble
        # is already clear, so without poison a port that did nothing matched. Reading a
        # byte is not the problem -- CHANGING WHAT THE FUNCTION DOES is, and that is
        # detected below by comparing the two runs' write sets.
        restore()
        poison = {}
        for seg, off in sorted(first):
            v = wrote[(seg, off)] ^ 0xFF
            m.write(seg, off, bytes([v]))
            poison['%d:%04x' % (seg, off)] = v
        o = probe.observe(m, CASE['shipped'], args, CASE.get('max_instr', 200000))
        second = written()
        if second != first:
            # The poison changed what the function DID, not just what it wrote over. That
            # happens when the bytes it scans are the bytes it writes -- append_period_spaces
            # is a strcat, and poisoning the terminator moves where the copy lands. For
            # such a set the poison is not a sharper comparison, it is a different
            # experiment, so it is dropped and the plain run is reported instead. The cost
            # is the one this poison exists to remove: a write of a value already there is
            # invisible again, for these arguments only.
            restore()
            o = probe.observe(m, CASE['shipped'], args, CASE.get('max_instr', 200000))
            poison, first = {}, written()
        finals = {}
        for seg, off in sorted(first | written()):
            finals['%d:%04x' % (seg, off)] = m.read(seg, off, 1)[0]
        out.append({'args': list(args), 'ax': o['return_dxax'] & 0xFFFF,
                    'dxax': o['return_dxax'], 'returned': o['reached_return'],
                    'poison': poison, 'final': finals})
    json.dump({'state': meta.get('state'), 'observations': out}, sys.stdout)


if __name__ == '__main__':
    main()
