#!/usr/bin/env python3
"""Emit the shared header this repo is allowed to have, from model/model.json.

The source repo cannot share a header: 18 of 18 far/near unifications and 32 of 44
base-type unifications STOP its byte match, because the shipped code really does reach
one address through DS here and ES there. This repo only has to BEHAVE the same, so one
declaration per object is not merely allowed, it is the point -- 268 addresses with
incompatible spellings collapse to 268 objects with one spelling each.

WHAT SURVIVES THE COLLAPSE, AND WHAT DOES NOT

  far/near   does not survive as ACCESS. Both spellings reach the same memory; the
             addressing mode was a byte-matching artefact and is dropped here.

  far        DOES survive as PLACEMENT, and this is the one place the source brief's
             "delete far" is too strong for a 16-bit target. DGROUP is 64 KiB and must
             also hold the heap and the stack; segment 20 alone runs to offset 0xded4
             (57,044) and the model's data totals ~88 KiB. So a global is defined `far`
             exactly when the shipped program put it outside DGROUP (segment != 34).
             That is placement, not semantics, and it is read from the model, not chosen.

  width      survives, because it is behaviour: signedness decides JL from JB. Where two
             spellings of one address differ in width this emits the WIDER one, so no
             site can over-read, and lists the narrower ones in model/derived.json for a
             cast at the site. Where they differ only in SIGNEDNESS at equal width this
             cannot pick from counting -- that has been wrong every time it was checked --
             so it emits one, marks it unresolved, and the report names the address.

  extent     is not in the model for an unsized array (`char far @[]`, 101 of them). It
             is inferred from the gap to the next global in the same segment, which is an
             UPPER bound and is marked as inferred. A wrong extent here is a link-time
             layout difference, not a silent read.

Overlapping globals are not a defect: SEG31:0x00a4 carries an array of edit buffers and
buffer 0 on purpose. Two names at one address must therefore be ONE object with an alias,
never two definitions, or the port has two buffers where the game has one.

    tools/gen_model_header.py           # -> include/col/*.h, model/derived.json
    tools/gen_model_header.py --check   # regenerate and fail if the checked-in copy moved
"""
import json, os, re, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
MODEL = os.path.join(REPO, 'model', 'model.json')
INC = os.path.join(REPO, 'include', 'col')
DERIVED = os.path.join(REPO, 'model', 'derived.json')

DGROUP = 34                      # the automatic data segment; everything else is far
# Borland's int is 16 bits and the program depends on it, so nothing generated here is
# spelt `int`. `char` stays `char`: signed under both toolchains as configured, and a
# string is still a string. See include/col/types.h.
SPELL = {'short': 'col_i16', 'unsigned short': 'col_u16',
         'int': 'col_i16', 'unsigned int': 'col_u16', 'unsigned': 'col_u16',
         'long': 'col_i32', 'unsigned long': 'col_u32',
         'signed char': 'col_i8'}


# Four records in the model are Windows' own, re-declared per file because a byte-exact
# translation unit could not include windows.h. Three of them (MSG, RGBQUAD, TEXTMETRIC)
# match the platform's layout, so the platform's win: the port calls the API with them and
# a second declaration would only be a way to disagree. Their MEMBER NAMES differ from the
# model's (`msg` is `message`, `x`/`y` are a POINT), which each porting site adapts.
#
# BITMAPINFO does NOT match. The model's is 1064 bytes -- a BITMAPINFOHEADER with a full
# 256-entry colour table, which is what the game actually allocates -- while windows.h
# declares 44. Taking the platform's there would overflow a real buffer, so this keeps the
# game's under a name of its own.
PLATFORM_RECORDS = ('MSG', 'RGBQUAD', 'TEXTMETRIC')
RENAMED_RECORDS = {'BITMAPINFO': 'ColBITMAPINFO'}


def spell(t):
    t = RENAMED_RECORDS.get(t, t)
    return SPELL.get(t, t)
QUAL = ('far', 'near', 'huge')
BASE_SIZE = {'char': 1, 'signed char': 1, 'unsigned char': 1,
             'short': 2, 'unsigned short': 2, 'int': 2, 'unsigned int': 2,
             'unsigned': 2, 'long': 4, 'unsigned long': 4, 'float': 4}
# Rank for picking one base type from several spellings of one address: wider wins,
# because a narrow view of a wide object is expressible as a cast at the site and a wide
# view of a narrow object is an over-read that nothing can express safely.
SIGNED_RANK = {'unsigned char': 0, 'char': 1, 'signed char': 2,
               'unsigned': 0, 'unsigned int': 0, 'int': 1, 'short': 1,
               'unsigned short': 0, 'unsigned long': 0, 'long': 1}


def parse(spelling):
    """'unsigned char far @[][0x10]' -> (base, stars, dims). '@' is the declarator."""
    head, _, tail = spelling.partition('@')
    dims = re.findall(r'\[([^\]]*)\]', tail)
    stars = head.count('*')
    words = [w for w in re.split(r'[\s*]+', head) if w and w not in QUAL]
    base = ' '.join(words)
    return base, stars, dims


def size_of(base, stars, dims, records):
    if stars:
        unit = 4                                   # every pointer here is far
    elif base in BASE_SIZE:
        unit = BASE_SIZE[base]
    elif base in records:
        unit = records[base]['size']
    elif base.startswith('struct '):
        return None                                # an opaque tag: no size to state
    else:
        return None
    n = 1
    for d in dims:
        if d == '':
            return None                            # unsized: the caller infers an extent
        n *= int(d, 0)
    return unit * n


def pointee_score(name, records):
    """Lower is better. A record whose members repeat a name is a MERGED BUCKET, not a
    type: `Rec` is 1,916 bytes and names one field `v` three times, because every file
    that called its anonymous struct `Rec` was unioned into it. Such a name loses to a
    record with distinct members even when it is bigger and has more of them."""
    r = records.get(name)
    if r is None:
        return (1, 0)
    names = [m['name'] for m in r['members']]
    dupes = len(names) - len(set(names))
    return (1 if dupes else 0, -len(set(names)))


def pick(spellings, records):
    """One canonical (base, stars, dims) for an address, plus what was discarded."""
    parsed = [parse(s) for s in spellings]
    notes = []
    stars = max(p[1] for p in parsed)
    if stars:
        # A pointer view wins over an integer one, and among POINTEES the widest-wins rule
        # above does not apply: every far pointer is 4 bytes, so the pointee's size says
        # nothing about which spelling is right. Take the record the corpus knows most
        # about instead -- `Colony` with 14 named members over a placeholder `Rec` with
        # three -- and note the one not taken. This is evidence, not a head count; a head
        # count has been wrong every time it was checked here.
        cands = [p for p in parsed if p[1] == stars]
        pointee = sorted(cands, key=lambda p: (pointee_score(p[0], records), p[0]))[0]
        others = {p[0] for p in cands if p[0] != pointee[0]}
        if others:
            notes.append(('other_pointee_spelling', sorted(others)))
        return (pointee[0], stars, pointee[2]), notes
    cands = parsed
    # widest base
    def w(p):
        return BASE_SIZE.get(p[0], records[p[0]]['size'] if p[0] in records else 0)
    best = max(w(p) for p in cands)
    wide = [p for p in cands if w(p) == best]
    # a record type beats a plain integer of the same width: it names the bytes
    named = [p for p in wide if p[0] not in BASE_SIZE]
    pool = named or wide
    # signedness at equal width cannot be settled by counting; take the signed spelling
    # (it is the Borland default for `char`) and SAY it is unsettled.
    pool = sorted(pool, key=lambda p: (-SIGNED_RANK.get(p[0], 1), -len(p[2]), p[0]))
    chosen = pool[0]
    others = {p[0] for p in parsed if p[0] != chosen[0]}
    if others:
        narrower = {o for o in others if BASE_SIZE.get(o, 99) < best}
        equal = {o for o in others
                 if BASE_SIZE.get(o) == best and o in BASE_SIZE and chosen[0] in BASE_SIZE}
        if narrower:
            notes.append(('narrow_view', sorted(narrower)))
        if equal:
            notes.append(('signedness_unresolved', sorted(equal | {chosen[0]})))
    # the widest dims among the spellings that agree on the base
    dims = max((p[2] for p in parsed if p[0] == chosen[0]), key=len)
    return (chosen[0], chosen[1], dims), notes


def declarator(name, stars, dims, far):
    d = name
    for x in dims:
        d += '[%s]' % x
    if stars:
        d = '*' * stars + ('far ' if True else '') + d
    return d


def decl(base, stars, dims, name, far):
    """`unsigned char far g_x[16]`, `void far * far g_p`, `Unit far g_units[]`."""
    base = spell(base)
    left = base
    if stars:
        left += ' far ' + '*' * stars
        left += 'far ' if far else ''
    else:
        left += ' far ' if far else ' '
    d = name + ''.join('[%s]' % x for x in dims)
    return (left + d).replace('  ', ' ')


# ----------------------------------------------------------------------- records

def record_text(name, rec, collisions):
    """One struct, holes padded, size asserted. A hole is a byte no file ever named."""
    size, members = rec['size'], sorted(rec['members'], key=lambda m: m['offset'])
    wc = {c['offset']: c for c in rec['width_conflicts']}
    nc = {c['offset']: c for c in rec['name_conflicts']}
    out = ['typedef struct {']
    at, used = 0, {}
    for m in members:
        off, mb = m['offset'], m['type']
        w = BASE_SIZE.get(mb, 1)
        if off < at:                    # an overlapping member: a narrow view of a wider
            out.append('    /* +0x%02x %s %s -- overlaps +0x%02x; read it as a cast */'
                       % (off, mb, m['name'], at - w))
            continue
        if off > at:
            out.append('    unsigned char _pad_%02x[%d];' % (at, off - at))
        member = m['name']
        if member in used:
            # One name, two offsets: two files used the same word for different fields and
            # the merge kept both. Renaming the later one is not a judgement about which is
            # right -- it is the minimum needed to compile, and the collision is reported.
            member = '%s_%x' % (member, off)
            collisions.append({'record': name, 'name': m['name'],
                               'offsets': [used[m['name']], off], 'emitted': member})
        used[member] = off
        cmt = []
        if member != m['name']:
            cmt.append('the model calls this %s too, at +0x%02x'
                       % (m['name'], used.get(m['name'], off)))
        if off in wc:
            cmt.append('widths seen: ' + ', '.join(wc[off]['types']))
        if off in nc:
            cmt.append('also named: ' + ', '.join(n for n in nc[off]['names']
                                                  if n != m['name']))
        out.append('    %-18s %-22s /* +0x%02x%s */'
                   % (spell(mb), member + ';', off,
                      '  ' + '; '.join(cmt) if cmt else ''))
        at = off + w
    if size > at:
        out.append('    unsigned char _pad_%02x[%d];' % (at, size - at))
    out.append('} %s;' % name)
    out.append('typedef char col_assert_%s[sizeof(%s) == %d ? 1 : -1];'
               % (name, name, size))
    return '\n'.join(out)


def main():
    model = json.load(open(MODEL))
    records, globals_ = model['records'], model['globals']
    prov = json.load(open(os.path.join(REPO, 'model', 'provenance.json')))

    # ---- records, emitted in dependency order so a record used by another comes first
    order, seen = [], set()

    def visit(n):
        if n in seen or n not in records:
            return
        seen.add(n)
        for m in records[n]['members']:
            if m['type'] not in BASE_SIZE:
                visit(m['type'])
        order.append(n)
    for n in sorted(records):
        visit(n)

    rh = ['/* GENERATED by tools/gen_model_header.py from model/model.json -- do not edit.',
          ' * model.json sha256 %s, exported from the byte-exact corpus at %s.'
          % (prov['model_sha256'][:16], prov['source_commit'][:12]),
          ' *',
          ' * %d records, holes padded, every size asserted at compile time. A member listed'
          % len(records),
          ' * as "widths seen" is read narrowly somewhere: express that as a cast at the site',
          ' * (*(unsigned char *)&rec.field), never as a second declaration.',
          ' */',
          '#ifndef COL_RECORDS_H', '#define COL_RECORDS_H', '',
          '#include "col/types.h"', '',
          '/* Byte alignment. Borland 16-bit defaults to it and every offset in the model',
          '   was measured under it; a host compiler would otherwise pad and the asserts',
          '   below would fail -- which is the point of the asserts. */',
          '#ifdef COL_HOST', '#  pragma pack(push, 1)',
          '#else', '#  pragma option -a1', '#endif', '']
    member_collisions = []
    for n in order:
        if n in PLATFORM_RECORDS:
            rh.append('/* %s: the platform\'s, from windows.h. The model agrees with it at '
                      '%d bytes;\n   its member names differ and each site adapts. */\n' %
                      (n, records[n]['size']))
            continue
        rh.append(record_text(RENAMED_RECORDS.get(n, n), records[n], member_collisions)
                  + '\n')
    rh += ['#ifdef COL_HOST', '#  pragma pack(pop)',
           '#else', '#  pragma option -a.', '#endif', '', '#endif']

    # ---- globals: group into OBJECTS first, then aliases into them.
    #
    # Two names at one address, or a name inside another's extent, must become ONE piece
    # of storage. Emitting both as `extern` would give the port two objects where the game
    # has one, and every write through the second would be invisible to the first. That is
    # the single most dangerous thing this generator could get wrong, so it is done by
    # extent arithmetic and reported, not assumed away.
    by_seg = {}
    for name, g in globals_.items():
        by_seg.setdefault(g['segment'], []).append((g['offset'], name, g))
    for s in by_seg:
        by_seg[s].sort(key=lambda e: (e[0], e[1]))

    info, inferred = {}, []
    for seg, entries in by_seg.items():
        offs = sorted({e[0] for e in entries})
        nxt = {o: n for o, n in zip(offs, offs[1:])}
        for off, name, g in entries:
            (base, stars, dims), notes = pick(g['types'], records)
            size, how = size_of(base, stars, dims, records), 'declared'
            if size is None and not dims:
                # a record whose body the model could not parse: it has no size to state,
                # so the name survives as a byte extent rather than as a wrong layout.
                size, how = None, 'opaque'
            if size is None:
                unit = size_of(base, stars, dims[1:], records) or 1
                if off in nxt:
                    size = nxt[off] - off
                    how = ('inferred: the gap to the next global' if how != 'opaque'
                           else 'opaque record; extent is the gap to the next global')
                else:
                    size, how = unit, how + '; inferred as one element (last in segment)'
                inferred.append(name)
            info[name] = dict(seg=seg, off=off, size=size, how=how, base=base,
                              stars=stars, dims=dims, notes=notes, addr=g['address'],
                              spellings=g['types'])

    def rank(name):
        """Which name owns the storage: the widest extent, then a named record over a
        plain integer, then the name itself so the choice is reproducible."""
        i = info[name]
        return (-i['size'], 0 if i['base'] not in BASE_SIZE else 1, name)

    objects, aliases, unresolved = [], [], []
    for seg in sorted(by_seg):
        cur = None
        for off, name, _ in by_seg[seg]:
            i = info[name]
            for kind, types in i['notes']:
                if kind == 'signedness_unresolved':
                    unresolved.append({'name': name, 'address': i['addr'],
                                       'types': types, 'emitted': i['base']})
            if cur is not None and off < cur['end']:
                cur['members'].append(name)
                cur['end'] = max(cur['end'], off + i['size'])
            else:
                cur = {'seg': seg, 'start': off, 'end': off + i['size'],
                       'members': [name]}
                objects.append(cur)
    for o in objects:
        o['primary'] = min(o['members'], key=rank)
        o['aliases'] = [n for n in o['members'] if n != o['primary']]
        for n in o['aliases']:
            aliases.append({'name': n, 'address': info[n]['addr'],
                            'aliases': o['primary'],
                            'delta': info[n]['off'] - info[o['primary']]['off'],
                            'type': info[n]['base'],
                            'reason': ('same address' if info[n]['off'] == o['start']
                                       else 'inside its extent')})

    # Two placements of the same 1,161 names, from the same arithmetic.
    #
    #   Win16   each object is an ordinary definition and the linker places it. `far`
    #           keeps the non-DGROUP data out of a 64 KiB DGROUP; the shipped ADDRESS is
    #           not reproduced and does not need to be, because nothing links against the
    #           shipped image.
    #
    #   host    each object is placed AT its shipped seg:off inside a slab, so a write the
    #           host makes and a write the emulator records are the same coordinate. That
    #           is what lets tests/differential.py compare a ported function against the
    #           shipped bytes without an OMF fixup resolver.
    win, host, defs = [], [], []
    for seg in sorted(by_seg):
        far = seg != DGROUP
        title = ('/* ---- %s %s */'
                 % ('SEG%d' % seg if far else 'DGROUP',
                    '-- far, outside the automatic data segment' if far
                    else '-- the automatic data segment'))
        win.append(title)
        host.append(title)
        for o in [x for x in objects if x['seg'] == seg]:
            p = o['primary']
            i = info[p]
            known = i['base'] in BASE_SIZE or i['base'] in records or i['stars']
            base = i['base'] if known else None
            cmt_extra = ''
            if base is None:            # an unparsed record: storage, named honestly
                dims, base, stars = ['%d' % i['size']], 'unsigned char', 0
                cmt_extra = 'the model could not parse %s; this is its extent' % i['base']
            else:
                dims, stars = i['dims'], i['stars']
                if dims and dims[0] == '':
                    unit = size_of(base, stars, dims[1:], records) or 1
                    dims = ['%d' % max(1, i['size'] // unit)] + dims[1:]
            cmt = i['addr']
            if cmt_extra:
                cmt += '; ' + cmt_extra
            if i['how'] != 'declared':
                cmt += '; ' + i['how']
            for k, t in i['notes']:
                cmt += '; %s: %s' % (k, ', '.join(t))
            d = decl(base, stars, dims, p, far)
            win.append('extern %-46s /* %s */' % (d + ';', cmt))
            defs.append('%s;' % d)
            for n in [p] + o['aliases']:
                a = info[n]
                cast = (spell(a['base']) + ' far ' + '*' * a['stars']).strip() \
                    if a['stars'] else spell(a['base'])
                if n == p:
                    cast, adims = (base + ' ' + '*' * stars).strip(), dims
                else:
                    adims = a['dims']
                at = 'col_seg(%d) + 0x%04x' % (seg, a['off'])
                body = ('((%s *)(%s))' if adims else '(*(%s *)(%s))') % (cast, at)
                host.append('#define %-36s %s' % (n, body))
            for n in o['aliases']:
                a = info[n]
                cast = (spell(a['base']) + ' far ' + '*' * a['stars']).strip() \
                    if a['stars'] else spell(a['base'])
                d = a['off'] - i['off']
                addr = ('(unsigned char far *)&%s + %d' % (p, d)) if d else \
                       ('(unsigned char far *)&%s' % p)
                body = (('((%s far *)(%s))' if a['dims'] else '(*(%s far *)(%s))')
                        % (cast, addr))
                win.append('#define %-38s %s  /* %s, %s */'
                           % (n, body, a['addr'],
                              'same address' if not d else 'inside %s' % p))
        win.append('')
        host.append('')
    lines = ['#ifdef COL_HOST', '',
             '/* Placed at the shipped addresses, in the slab tests/host/colmem.c owns. */',
             '#include "col/colmem.h"', ''] + host + \
            ['#else', ''] + win + ['#endif']

    gh = ['/* GENERATED by tools/gen_model_header.py from model/model.json -- do not edit.',
          ' *',
          ' * %d names over %d objects. The address in each comment is where the SHIPPED'
          % (len(globals_), len(objects)),
          ' * program kept it, checked there against 55,950 reference sites; here it is',
          ' * identity and evidence, not linkage -- this repo allocates its own storage.',
          ' *',
          ' * %d names are not objects of their own but views into one: the same address'
          % len(aliases),
          ' * under a second name, or a byte inside a word. They are macros so that a write',
          ' * through either name is seen by the other, which two `extern`s would not give.',
          ' *',
          ' * `far` follows the shipped segment. That is placement, not access: DGROUP is',
          ' * 64 KiB and must also hold the heap and the stack, while the model\'s data',
          ' * totals about 88 KiB, so the data outside DGROUP has to stay outside it.',
          ' */',
          '#ifndef COL_GLOBALS_H', '#define COL_GLOBALS_H', '',
          '#include "col/win16.h"', '#include "col/records.h"', ''] + lines + ['#endif']

    world = ['/* GENERATED by tools/gen_model_header.py -- do not edit.',
             ' *',
             ' * The storage behind col/globals.h for the Win16 build: one definition per',
             ' * object, `far` for everything the shipped program kept outside DGROUP.',
             ' * The host build does not use this file -- there the slab is the storage.',
             ' *',
             ' * NOT YET INITIALISED. %d of these objects ship with CONTENTS in the'
             % len(objects),
             ' * binary\'s data segments -- tables the game reads before anything writes',
             ' * them -- and zeroed storage is a behaviour difference, not a detail. The',
             ' * initialisers come from the shipped image; see README, "What is not done".',
             ' */',
             '#ifndef COL_HOST',
             '#include "col/globals.h"', ''] + defs + ['#endif']

    os.makedirs(INC, exist_ok=True)
    os.makedirs(os.path.join(REPO, 'src'), exist_ok=True)
    changed = []
    for path, text in ((os.path.join(INC, 'records.h'), '\n'.join(rh) + '\n'),
                       (os.path.join(INC, 'globals.h'), '\n'.join(gh) + '\n'),
                       (os.path.join(REPO, 'src', 'world.c'), '\n'.join(world) + '\n'),
                       (DERIVED, json.dumps({
                           'what': 'what the header generator concluded, and what it could not',
                           'model_sha256': prov['model_sha256'],
                           'globals': info,
                           'objects': objects,
                           'aliases': aliases,
                           'signedness_unresolved': unresolved,
                           'member_name_collisions': member_collisions,
                           'platform_records': list(PLATFORM_RECORDS),
                           'renamed_records': RENAMED_RECORDS,
                           'inferred_extents': sorted(inferred)}, indent=1) + '\n')):
        old = open(path).read() if os.path.exists(path) else None
        if old != text:
            changed.append(os.path.relpath(path, REPO))
            if '--check' not in sys.argv:
                open(path, 'w').write(text)
    print('%d records (%d member-name collisions renamed); %d globals over %d objects; '
          '%d aliases; %d signedness unresolved; %d inferred extents'
          % (len(records), len(member_collisions), len(globals_), len(objects),
             len(aliases), len(unresolved), len(inferred)))
    if '--check' in sys.argv and changed:
        print('STALE: ' + ', '.join(changed), file=sys.stderr)
        return 1
    print('wrote ' + ', '.join(changed) if changed else 'up to date')
    return 0


if __name__ == '__main__':
    sys.exit(main())
