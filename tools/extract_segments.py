#!/usr/bin/env python3
"""Lift the shipped program's initialised data out of COLONIZE.EXE.

WHY. Defining every global as zeroed storage, which is what a port does if it only reads
the model, is a behaviour difference rather than a detail: most of these objects ship with CONTENTS -- the unit table, the
terrain table, every string the game prints -- which the program reads before anything
writes them. A port with zeros there is a different program.

WHAT IT EMITS, and why it is a segment image rather than 1,034 initialisers.

Each of the 15 DATA segments becomes one array with the shipped bytes, and every global
becomes a macro into it at its shipped offset. That is a bigger change than adding
initialisers, and it buys three things:

  * the 360 globals whose extent this repo had to INFER from the gap to the next global
    stop mattering. The inference is an upper bound and could be wrong; with the segment
    image intact, a global whose real array is longer than the guess still has its real
    bytes behind it, and a walk off the end reads what it read in 1995.
  * the relative order of everything in a segment is the shipped order, not whatever the
    linker chose. Sixteen-bit game code walks between adjacent tables.
  * one spelling serves both targets. The host harness places data at the shipped
    addresses so it can be compared with the emulator; now the Win16 build does too.

WHAT IT CANNOT FINISH. 193 four-byte sites in DGROUP and SEG31 are RELOCATIONS: far
pointers the loader fills in, so the file holds a chain offset there rather than an
address. 93 point into data and are emitted as runtime patches in col_relocate(). The
other 100 point at CODE -- function pointers in dispatch tables -- and cannot be written
until those functions are ported. They are listed in model/relocations.json with the name
each one wants, and col_relocate() leaves them alone.

    tools/extract_segments.py                # -> src/segdata.c, include/col/segments.h
    tools/extract_segments.py --verify       # compare every byte against the emulator
"""
import json, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
SRC_REPO = os.environ.get('COL_SRC_REPO',
                          os.path.expanduser('~/Repositories/dderevjanik/col-win-re'))
EXE = os.path.join(SRC_REPO, 'game', 'COLONIZE.EXE')
RELOCS = os.path.join(SRC_REPO, 'analysis', 'ne', 'COLONIZE.EXE.relocs.json')
SYMBOLS = os.path.join(SRC_REPO, 're', 'symbols.json')
DGROUP = 34


def ne_segments(path):
    """[(index, file_offset, length, flags, min_alloc)] straight out of the NE header.

    Only the segment TABLE is parsed here. The relocation CHAINS are not: a non-additive
    record stores the head of a chain inside the segment data and each link holds the next
    offset, which is exactly the kind of thing that is easy to get subtly wrong. The byte-
    exact repo already resolved them, this reads its answer, and --verify checks the whole
    result against an independent loader (the emulator) rather than trusting either.
    """
    b = open(path, 'rb').read()
    ne = struct.unpack_from('<H', b, 0x3c)[0]
    if b[ne:ne + 2] != b'NE':
        raise SystemExit('%s is not an NE image' % path)
    count = struct.unpack_from('<H', b, ne + 0x1c)[0]
    table = ne + struct.unpack_from('<H', b, ne + 0x22)[0]
    shift = struct.unpack_from('<H', b, ne + 0x32)[0]
    out = []
    for i in range(count):
        sector, length, flags, minalloc = struct.unpack_from('<4H', b, table + i * 8)
        out.append((i + 1, sector << shift, length or 0x10000, flags, minalloc or 0x10000))
    return b, out


def reloc_sites():
    """{(segment, offset): target} for every relocation in a DATA segment."""
    recs = json.load(open(RELOCS))['relocations']
    out = {}
    for rec in recs:
        for s in rec['sites']:
            out[(rec['segment'], s)] = rec
    return out


def symbols():
    return json.load(open(SYMBOLS))['labels']


def carray(name, data, far, per_line=16):
    head = 'unsigned char %s%s[%dL] = {' % ('far ' if far else '', name, len(data))
    rows = []
    for i in range(0, len(data), per_line):
        rows.append('    ' + ','.join('%d' % x for x in data[i:i + per_line]) + ',')
    return head + '\n' + '\n'.join(rows) + '\n};'


def main():
    image, segs = ne_segments(EXE)
    sites = reloc_sites()
    syms = symbols()
    data_segs = [s for s in segs if s[3] & 1]        # bit 0 clear = CODE, set = DATA
    ids = [s[0] for s in data_segs]

    patches, pending = [], []
    for (seg, off), rec in sorted(sites.items()):
        if seg not in ids:
            continue
        t = rec['target']
        entry = {'segment': seg, 'offset': off, 'addr_type': rec['addr_type'],
                 'target_segment': t['segment'], 'target_offset': t['offset']}
        if t['segment'] in ids:
            patches.append(entry)
        else:
            entry['symbol'] = syms.get('L%x_%04x' % (t['segment'], t['offset']))
            pending.append(entry)

    decls, defs = [], []
    for idx, foff, length, flags, minalloc in data_segs:
        blob = image[foff:foff + length]
        if len(blob) != length:
            raise SystemExit('segment %d runs past the end of the file' % idx)
        far = idx != DGROUP
        decls.append('extern unsigned char %scol_seg%d[%dL];'
                     % ('far ' if far else '', idx, length))
        defs.append(carray('col_seg%d' % idx, blob, far))

    table = '\n'.join('    case %d: return col_seg%d;' % (i, i) for i in ids)
    sizes = '\n'.join('    case %d: return %dL;' % (s[0], s[2]) for s in data_segs)
    # On Win16 a far pointer IS four bytes and the assignment is the whole story. On the
    # host it is eight, so assigning one would write four bytes past the site and smash
    # whatever the model names there -- which it did: g_line_color_index lives one byte
    # past a relocation site, and a host-pointer store silently changed it. The host
    # therefore writes the 16:16 ENCODING instead, selector and all, so the four bytes
    # match what the emulator holds. Nothing on the host can dereference such a pointer,
    # and nothing should: a far pointer is not representable there.
    patch_lines = '\n'.join(
        '    *(void far **)(col_seg%d + 0x%04xL) = '
        '(void far *)(col_seg%d + 0x%04xL);'
        % (p['segment'], p['offset'], p['target_segment'], p['target_offset'])
        for p in patches)
    host_lines = '\n'.join(
        '    col_far(col_seg%d + 0x%04xL, 0x%04x, 0x%04x);'
        % (p['segment'], p['offset'], 0x1000 + (p['target_segment'] - 1) * 8,
           p['target_offset'] & 0xFFFF)
        for p in patches)

    src = '''/* GENERATED by tools/extract_segments.py from game/COLONIZE.EXE -- do not edit.
 *
 * The %d DATA segments of the shipped image, byte for byte, %d bytes in all. Every
 * global in col/globals.h is a macro into one of these at its shipped offset, so the
 * port's data has the shipped CONTENTS and the shipped ORDER.
 *
 * Extracted, never hand-written: tools/extract_segments.py --verify compares all %d
 * bytes against the same image loaded by an independent loader (the byte-exact repo's
 * emulator) and requires the only differences to be the %d relocation sites below.
 */
#include "col/segments.h"

%s

unsigned char far *col_seg(int seg)
{
    switch (seg) {
%s
    }
    return 0;
}

long col_seg_size(int seg)
{
    switch (seg) {
%s
    }
    return 0;
}

/* The %d relocations whose target is DATA. The loader filled these in; nothing else
 * does, so col_relocate() must run before anything reads them. The other %d point at
 * CODE -- function pointers in dispatch tables -- and are listed in
 * model/relocations.json with the name each one wants. They stay as the file left them
 * until the functions they name are ported. */
#ifdef COL_HOST
static void col_far(unsigned char *at, unsigned sel, unsigned off)
{
    at[0] = (unsigned char)(off & 0xff);
    at[1] = (unsigned char)(off >> 8);
    at[2] = (unsigned char)(sel & 0xff);
    at[3] = (unsigned char)(sel >> 8);
}
#endif

void col_relocate(void)
{
#ifdef COL_HOST
%s
#else
%s
#endif
}
''' % (len(data_segs), sum(s[2] for s in data_segs), sum(s[2] for s in data_segs),
       len(patches) + len(pending), '\n\n'.join(defs), table, sizes,
       len(patches), len(pending), host_lines, patch_lines)

    hdr = '''/* GENERATED by tools/extract_segments.py -- do not edit.
 *
 * The shipped data segments. col/globals.h names the objects inside them; this is the
 * storage. col_relocate() fills in the far pointers the loader used to fill in and MUST
 * run before anything else -- see src/segdata.c.
 */
#ifndef COL_SEGMENTS_H
#define COL_SEGMENTS_H

#include "col/win16.h"      /* `far` is a keyword there and erased on the host */

%s

unsigned char far *col_seg(int seg);      /* 0 for a segment that is not DATA */
long col_seg_size(int seg);
void col_relocate(void);

#endif
''' % '\n'.join(decls)

    out = [(os.path.join(REPO, 'src', 'segdata.c'), src),
           (os.path.join(REPO, 'include', 'col', 'segments.h'), hdr),
           (os.path.join(REPO, 'model', 'segments.json'),
            json.dumps({'what': 'the shipped data segments and their lengths',
                        'sizes': {str(s[0]): s[2] for s in data_segs}}, indent=1) + '\n'),
           (os.path.join(REPO, 'model', 'relocations.json'),
            json.dumps({'what': 'far pointers the Windows loader used to fill in',
                        'resolved_here': patches,
                        'need_a_ported_function': pending}, indent=1) + '\n')]
    changed = []
    for path, text in out:
        old = open(path).read() if os.path.exists(path) else None
        if old != text:
            changed.append(os.path.relpath(path, REPO))
            if '--check' not in sys.argv:
                open(path, 'w').write(text)
    print('%d data segments, %d bytes; %d relocations resolved, %d awaiting a ported '
          'function' % (len(data_segs), sum(s[2] for s in data_segs),
                        len(patches), len(pending)))
    if '--check' in sys.argv and changed:
        print('STALE: ' + ', '.join(changed), file=sys.stderr)
        return 1
    print('wrote ' + ', '.join(changed) if changed else 'up to date')
    return 0


if __name__ == '__main__':
    sys.exit(main())
