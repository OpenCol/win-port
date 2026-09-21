#!/usr/bin/env python3
"""Is every ported function checked by SOMETHING?

Two harnesses, each blind in its own way. The host one cannot see a function whose whole
effect is a call, cannot represent a far pointer, and links its own C library. The 16-bit
one cannot place a candidate in a callee's own segment, so it cannot run a rule-47
forwarder. Either can report a function as uncheckable and be right.

What must not happen is a function that NEITHER checks sitting in src/ looking ported. This
reads both result files and says so.

    tests/differential.py --all && tests/emu_differential.py --all && tools/coverage.py
"""
import json, os, sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
B = os.path.join(REPO, 'build', 'diff')


def main():
    try:
        host = json.load(open(os.path.join(B, 'host_results.json')))
        emu = json.load(open(os.path.join(B, 'emu_results.json')))
    except FileNotFoundError as e:
        print('run both harnesses with --all first (%s)' % e.filename)
        return 2
    host = {k: (v if isinstance(v, dict) else {'status': v, 'why': ''})
            for k, v in host.items()}
    emu = {k: (v if isinstance(v, dict) else {'status': v, 'why': ''})
           for k, v in emu.items()}
    st_h = {k: v['status'] for k, v in host.items()}
    st_e = {k: v['status'] for k, v in emu.items()}
    names = sorted(set(host) | set(emu))
    both = [n for n in names if st_h.get(n) == 'agreed' and st_e.get(n) == 'agreed']
    host_only = [n for n in names if st_h.get(n) == 'agreed' and st_e.get(n) != 'agreed']
    emu_only = [n for n in names if st_e.get(n) == 'agreed' and st_h.get(n) != 'agreed']
    neither = [n for n in names if st_h.get(n) != 'agreed' and st_e.get(n) != 'agreed']
    bad = [n for n in names if 'disagreed' in (st_h.get(n), st_e.get(n))]
    print('%d ported functions' % len(names))
    print('  %4d verified by both harnesses' % len(both))
    print('  %4d verified on the host only (the 16-bit one cannot run them)'
          % len(host_only))
    print('  %4d verified in 16 bits only (the host cannot judge them)' % len(emu_only))
    # An uncovered function is acceptable when BOTH harnesses said why they could not
    # judge it; the reasons are written to model/coverage.json. One that is uncovered
    # with no reason given is a hole, and that is what this fails on.
    news = [n for n in neither
            if not (host.get(n, {}).get('why') and emu.get(n, {}).get('why'))]
    print('  %4d verified by NEITHER, each with a reason from both harnesses'
          % (len(neither) - len(news)))
    if news:
        print('  %4d UNCOVERED AND UNEXPLAINED' % len(news))
    if bad:
        print('  %4d DISAGREE with the shipped bytes: %s' % (len(bad), ', '.join(bad[:5])))
    for n in news[:20]:
        print('       %-28s host: %-11s 16-bit: %s'
              % (n, st_h.get(n, '-'), st_e.get(n, '-')))
    json.dump({'what': 'which harness checks each ported function',
               'both': both, 'host_only': host_only, 'emu_only': emu_only,
               'neither': {n: {'host': host.get(n, {}).get('why', ''),
                               'sixteen_bit': emu.get(n, {}).get('why', '')}
                           for n in neither}},
              open(os.path.join(REPO, 'model', 'coverage.json'), 'w'), indent=1)
    # An uncovered function is tolerable when it is named and explained; a NEW one is not.
    return 1 if (bad or news) else 0


if __name__ == '__main__':
    sys.exit(main())
