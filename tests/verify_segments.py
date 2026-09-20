#!/usr/bin/env python3
"""Is src/segdata.c really the shipped image?

Runs under the byte-exact repo's interpreter. It loads COLONIZE.EXE with an INDEPENDENT
loader -- the emulator, which parses the NE header, walks the relocation chains and
applies them, none of which tools/extract_segments.py does -- and compares every byte of
every data segment against what this repo extracted.

The requirement is not "no differences". It is that the differences are EXACTLY the
relocation sites: four bytes at each, and nowhere else. Anything else means the extraction
read the wrong file offset, or the relocation list is short, and either would put wrong
data in the port silently.
"""
import json, os, sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC = sys.argv[1] if len(sys.argv) > 1 else \
    os.path.expanduser('~/Repositories/dderevjanik/col-win-re')
sys.path.insert(0, os.path.join(SRC, 'tools', 're_agent'))
sys.path.insert(0, os.path.join(SRC, 'tools'))
import probe  # noqa: E402

import re
BLOB = re.compile(r'unsigned char (?:far )?col_seg(\d+)\[(\d+)L\] = \{(.*?)\n\};', re.S)


def extracted():
    text = open(os.path.join(REPO, 'src', 'segdata.c')).read()
    out = {}
    for m in BLOB.finditer(text):
        body = m.group(3).replace('\n', '').strip().rstrip(',')
        out[int(m.group(1))] = bytes(int(x) for x in body.split(','))
    return out


def main():
    blobs = extracted()
    rel = json.load(open(os.path.join(REPO, 'model', 'relocations.json')))
    sites = {(p['segment'], p['offset'])
             for p in rel['resolved_here'] + rel['need_a_ported_function']}
    expected = {(s, o + k) for s, o in sites for k in range(4)}

    m, _ = probe.build('shipped', [])
    # The emulator relocates a segment when something first reaches it, so a segment no
    # code has touched still holds the file's chain bytes. Force all of them, or SEG31's
    # three sites look like bytes the loader never fills in -- which is how they first
    # read, and they are not.
    m.relocate_all()
    bad, seen = 0, set()
    for seg in sorted(blobs):
        ours = blobs[seg]
        theirs = m.read(seg, 0, len(ours))
        for i in range(len(ours)):
            if ours[i] != theirs[i]:
                if (seg, i) in expected:
                    seen.add((seg, i))
                    continue
                bad += 1
                if bad <= 20:
                    print('  SEG%d:0x%04x extracted %d, the loader has %d'
                          % (seg, i, ours[i], theirs[i]))
    print('%d segments, %d bytes compared' % (len(blobs), sum(len(b) for b in blobs.values())))
    print('%d unexplained differences; %d of %d relocation bytes differ as they should'
          % (bad, len(seen), len(expected)))
    # A relocation byte that does NOT differ is not an error -- a chain's last link or a
    # low byte can coincide -- but a WHOLE site that never differs would mean the site
    # list named a place the loader never touched, so report that separately.
    quiet = sorted({(s, o) for s, o in sites
                    if not any((s, o + k) in seen for k in range(4))})
    if quiet:
        print('%d relocation sites the loader did not change: %s'
              % (len(quiet), quiet[:8]))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
