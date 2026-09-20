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
    out = []
    for args in argsets:
        o = probe.observe(m, CASE['shipped'], args, CASE.get('max_instr', 200000))
        touched = {}
        for w in m.log['writes']:
            if w.get('space') in ('stack', 'scratch') or not w.get('addr'):
                continue
            sel, _, off = w['addr'].partition(':')
            seg = (int(sel, 16) - 0x1000) // 8 + 1
            for k in range(w.get('size', 1)):
                touched['%d:%04x' % (seg, int(off, 16) + k)] = \
                    m.read(seg, int(off, 16) + k, 1)[0]
        out.append({'args': list(args), 'ax': o['return_dxax'] & 0xFFFF,
                    'dxax': o['return_dxax'], 'returned': o['reached_return'],
                    'final': touched})
    json.dump({'state': meta.get('state'), 'observations': out}, sys.stdout)


if __name__ == '__main__':
    main()
