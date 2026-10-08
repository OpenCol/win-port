/* The game's initialised data, from the player's COLONIZE.EXE (roadmap W4).
 *
 * In the shipped program every global starts with what its data segment holds in the file:
 * the direction tables, _ctype, the key map, the resource type tags, the default fonts. The
 * port defines those globals (src/gen) but the values are the game's own bytes, which are
 * never committed; so at startup, before any game constructor, this reads them from
 * COLONIZE.EXE in the working directory (the game's) and converts each global as
 * tests/state.py does:
 *   - the bytes, where the port lays the global out as the game does (records, classes);
 *   - Borland's 2-byte int widened where the port still declares `int`;
 *   - a far pointer the loader patched (the segment's relocation records: FAR_ADDR, internal)
 *     mapped to the native address: a function by its address (src/gen's function table), a
 *     global by its address, anything else (a string literal) to a private copy of its
 *     segment; a pointer slot with no relocation keeps its 4 bytes (resource type tags).
 * The table of globals and functions is src/gen/globals_init.c (tools/gen_globals.py).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "w16.h"

struct PortInitGlobal { void *addr; unsigned short seg, off, count, w16, wn, kind; };
struct PortInitFunction { unsigned long addr16; void *fn; };
extern const struct PortInitGlobal port_init_globals[];
extern const unsigned port_init_global_count;
extern const struct PortInitFunction port_init_functions[];
extern const unsigned port_init_function_count;

#define MAX_SEGS 64
static struct { unsigned char *data; unsigned length; int is_data; unsigned char *shadow; } segs[MAX_SEGS + 1];
static unsigned nsegs;
/* relocations in data segments: (seg, off) -> target (seg, off) */
static struct { unsigned short seg, off, tseg, toff; } fix[1024];
static unsigned nfix;

static unsigned r16(const unsigned char *p) { return p[0] | p[1] << 8; }

static int read_image(const char *path)
{
    FILE *f = fopen(path, "rb");
    unsigned char *exe;
    long size;
    unsigned ne, segtab, shift, i;
    if (!f)
        return 0;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    exe = malloc((size_t)size);
    if (fread(exe, 1, (size_t)size, f) != (size_t)size) {
        fclose(f);
        return 0;
    }
    fclose(f);
    ne = r16(exe + 0x3c);
    nsegs = r16(exe + ne + 0x1c);
    segtab = ne + r16(exe + ne + 0x22);
    shift = r16(exe + ne + 0x32);
    if (nsegs > MAX_SEGS)
        nsegs = MAX_SEGS;
    for (i = 0; i < nsegs; i++) {
        const unsigned char *e = exe + segtab + i * 8;
        unsigned off = r16(e), len = r16(e + 2), flags = r16(e + 4), minalloc = r16(e + 6);
        unsigned alloc = minalloc ? minalloc : 0x10000;
        unsigned n = len ? len : (off ? 0x10000 : 0);
        segs[i + 1].is_data = flags & 1;
        if (!segs[i + 1].is_data)
            continue;
        segs[i + 1].data = calloc(1, alloc > n ? alloc : n);
        segs[i + 1].length = alloc > n ? alloc : n;
        if (off)
            memcpy(segs[i + 1].data, exe + ((size_t)off << shift), n);
        if (off && (flags & 0x100)) {
            const unsigned char *rel = exe + ((size_t)off << shift) + n;
            unsigned k, count = r16(rel);
            for (k = 0; k < count && nfix < 1024; k++) {
                const unsigned char *rr = rel + 2 + k * 8;
                if (rr[0] == 3 && (rr[1] & 3) == 0 && rr[4] != 0xff) {   /* far address, internal, fixed seg */
                    unsigned src = r16(rr + 2);
                    /* a chain: each slot's offset field holds the next slot, 0xffff ends it */
                    while (src != 0xffff && src + 4 <= n && nfix < 1024) {
                        unsigned next = r16(segs[i + 1].data + src);
                        fix[nfix].seg = (unsigned short)(i + 1); fix[nfix].off = (unsigned short)src;
                        fix[nfix].tseg = rr[4]; fix[nfix].toff = (unsigned short)r16(rr + 6);
                        nfix++;
                        if (rr[1] & 4)            /* additive: no chain */
                            break;
                        src = next;
                    }
                }
            }
        }
    }
    free(exe);
    return 1;
}

static void *function_at(unsigned seg, unsigned off)
{
    unsigned long want = ((unsigned long)(0x1000 + (seg - 1) * 8) << 16) | off;
    unsigned lo = 0, hi = port_init_function_count;
    while (lo < hi) {
        unsigned mid = (lo + hi) / 2;
        if (port_init_functions[mid].addr16 < want)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo < port_init_function_count && port_init_functions[lo].addr16 == want ? port_init_functions[lo].fn : 0;
}

static void *map_far(unsigned tseg, unsigned toff)
{
    unsigned i;
    if (tseg > nsegs)
        return 0;
    if (!segs[tseg].is_data)
        return function_at(tseg, toff);
    for (i = 0; i < port_init_global_count; i++) {
        const struct PortInitGlobal *g = &port_init_globals[i];
        unsigned span = (unsigned)g->count * g->w16;
        if (g->seg == tseg && toff >= g->off && toff < g->off + span) {
            unsigned rel = toff - g->off;
            if (g->w16 == g->wn)
                return (char *)g->addr + rel;
            return (char *)g->addr + (rel / g->w16) * g->wn;
        }
    }
    /* no global: a literal; point into a private copy of its segment */
    if (!segs[tseg].shadow) {
        segs[tseg].shadow = malloc(segs[tseg].length);
        memcpy(segs[tseg].shadow, segs[tseg].data, segs[tseg].length);
    }
    return segs[tseg].shadow + toff;
}

static int fixup_at(unsigned seg, unsigned off, unsigned *tseg, unsigned *toff)
{
    unsigned i;
    for (i = 0; i < nfix; i++)
        if (fix[i].seg == seg && fix[i].off == off) {
            *tseg = fix[i].tseg;
            *toff = fix[i].toff;
            return 1;
        }
    return 0;
}

__attribute__((constructor(101))) static void port_load_initial_data(void)
{
    unsigned i, k, loaded = 0, unmapped = 0;
    if (!read_image("COLONIZE.EXE")) {
        w16_trace("initial data: COLONIZE.EXE not found in the working directory");
        return;
    }
    for (i = 0; i < port_init_global_count; i++) {
        const struct PortInitGlobal *g = &port_init_globals[i];
        const unsigned char *src;
        unsigned char *dst = g->addr;
        if (g->seg > nsegs || !segs[g->seg].data || g->off + (unsigned)g->count * g->w16 > segs[g->seg].length)
            continue;
        src = segs[g->seg].data + g->off;
        for (k = 0; k < g->count; k++, src += g->w16, dst += g->wn) {
            unsigned tseg, toff;
            if ((g->kind == 1 || g->kind == 2) && g->w16 > 2 && g->wn == 2 * g->w16) {
                /* a row of ints (int g_group_members[7][8]): each of its words widened */
                unsigned j;
                for (j = 0; j < g->w16; j += 2) {
                    int32_t v = g->kind == 1 ? (int32_t)(int16_t)r16(src + j) : (int32_t)r16(src + j);
                    memcpy(dst + 2 * j, &v, 4);
                }
            } else if ((g->kind == 1 || g->kind == 2) && g->w16 != g->wn) {   /* int: Borland's width, widened */
                long v;
                if (g->w16 == 2)
                    v = g->kind == 1 ? (long)(int16_t)r16(src) : (long)r16(src);
                else if (g->w16 == 1)
                    v = g->kind == 1 ? (long)(int8_t)src[0] : (long)src[0];
                else
                    v = (long)(r16(src) | (unsigned long)r16(src + 2) << 16);
                memcpy(dst, &v, g->wn < sizeof v ? g->wn : sizeof v);
            } else if (g->w16 == g->wn) {
                memcpy(dst, src, g->wn);
            } else {
                continue;                                /* a layout the loader does not convert */
            }
        }
        loaded++;
    }
    /* far pointers the loader patched: wherever they sit in a global laid out as the game's */
    for (i = 0; i < nfix; i++) {
        unsigned j;
        for (j = 0; j < port_init_global_count; j++) {
            const struct PortInitGlobal *g = &port_init_globals[j];
            unsigned span = (unsigned)g->count * g->w16;
            if (g->seg == fix[i].seg && fix[i].off >= g->off && fix[i].off + 4 <= g->off + span && g->w16 == g->wn) {
                void *p = map_far(fix[i].tseg, fix[i].toff);
                if (!p)
                    unmapped++;
                memcpy((char *)g->addr + (fix[i].off - g->off), &p, sizeof p);
                break;
            }
        }
    }
    w16_trace("initial data: %u globals from COLONIZE.EXE, %u relocations (%u to no function)", loaded, nfix, unmapped);
    (void)fixup_at;
}
