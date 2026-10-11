/* The game's initialised data (roadmap W4, W10).
 *
 * In the shipped program every global starts with what its data segment holds in the file:
 * the direction tables, _ctype, the key map, the resource type tags, the default fonts. The
 * port defines those globals (src/gen) and, at startup, before any game constructor, fills
 * them from src/gen/exe_data.c -- the bytes tools/gen_exe_data.py took from the patched
 * COLONIZE.EXE (until 2026-10-10 this read the player's file) -- converting each global as
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
struct PortExeSeg { unsigned char is_data; unsigned length; };
struct PortExeBytes { unsigned short seg, off, len; const unsigned char *bytes; };
struct PortExeFix { unsigned short seg, off, tseg, toff; };
extern const unsigned port_exe_seg_count, port_exe_byte_count, port_exe_fix_count;
extern const struct PortExeSeg port_exe_segs[];
extern const struct PortExeBytes port_exe_bytes[];
extern const struct PortExeFix port_exe_fixes[];
struct PortExeAddress { unsigned short seg, off; void *addr; };
extern const unsigned port_exe_address_count;
extern const struct PortExeAddress port_exe_addresses[];

#define MAX_SEGS 64
static struct { unsigned char *data; unsigned length; int is_data; unsigned char *shadow; } segs[MAX_SEGS + 1];
static unsigned nsegs;
/* relocations in data segments: (seg, off) -> target (seg, off) */
static struct { unsigned short seg, off, tseg, toff; } fix[1024];
static unsigned nfix;

static unsigned r16(const unsigned char *p) { return p[0] | p[1] << 8; }

/* the process heap, not the C runtime: this runs as a constructor, and in the Linux build
   (Winelib, W8) a constructor runs before Wine's msvcrt is set up -- malloc is not ready yet,
   HeapAlloc is */
static void *zalloc(size_t n)
{
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, n ? n : 1);
}

/* the image as the Win16 loader left it, from src/gen/exe_data.c (tools/gen_exe_data.py): each
   data segment zeroed to its loaded size, then the bytes of the globals that start non-zero,
   and the relocations inside them */
static int load_compiled(void)
{
    unsigned i;
    nsegs = port_exe_seg_count > MAX_SEGS ? MAX_SEGS : port_exe_seg_count;
    for (i = 1; i <= nsegs; i++) {
        segs[i].is_data = port_exe_segs[i].is_data;
        segs[i].length = port_exe_segs[i].length;
        if (segs[i].is_data)
            segs[i].data = zalloc(segs[i].length);
    }
    for (i = 0; i < port_exe_byte_count; i++) {
        const struct PortExeBytes *b = &port_exe_bytes[i];
        if (b->seg <= nsegs && segs[b->seg].data && b->off + (unsigned)b->len <= segs[b->seg].length)
            memcpy(segs[b->seg].data + b->off, b->bytes, b->len);
    }
    for (i = 0; i < port_exe_fix_count && nfix < 1024; i++, nfix++) {
        fix[nfix].seg = port_exe_fixes[i].seg; fix[nfix].off = port_exe_fixes[i].off;
        fix[nfix].tseg = port_exe_fixes[i].tseg; fix[nfix].toff = port_exe_fixes[i].toff;
    }
    return nsegs != 0;
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
    /* one of the port's constructed objects (g_font12, g_main_win): globals_init.c does not list
       them, src/gen/exe_data.c does -- without this, a pointer to one pointed at a copy of the
       file's bytes, and Popup_start's g_popup_font_small (g_font12) read a NULL font there:
       Jukebox > By Song crashed on Windows 7 */
    for (i = 0; i < port_exe_address_count; i++)
        if (port_exe_addresses[i].seg == tseg && port_exe_addresses[i].off == toff && port_exe_addresses[i].addr)
            return port_exe_addresses[i].addr;
    /* no global: a literal; point into a private copy of its segment */
    if (!segs[tseg].shadow) {
        segs[tseg].shadow = zalloc(segs[tseg].length);
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

/* how many globals the loader filled (-1: not loaded): the Linux build's start
   reports it, its trace from here being too early to write */
int w16_initial_loaded = -1;

#ifndef __WINE__
__attribute__((constructor(101))) static
#endif
void port_load_initial_data(void)
{
    unsigned i, k, loaded = 0, unmapped = 0;
#ifndef __WINE__
    w16_crash_install();   /* the crash report first: a fault here must be reported (w16_crash.c) */
#endif
    if (!load_compiled()) {
        w16_report("initial data: src/gen/exe_data.c is empty");
        return;
    }
    /* $PORT_INIT_REPORT: each global whose layout this cannot convert, to $PORT_LOG (W9) */
    int report = GetEnvironmentVariableA("PORT_INIT_REPORT", 0, 0) != 0;
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
            } else if (g->w16 == 4 && g->wn == 8) {
                /* a pointer on a 64-bit target (W9): the game's 4 bytes, zero-extended -- a null
                   pointer, or a value kept in a pointer (g_picture_resource_type's 'CvPc');
                   a far pointer the loader relocated is set below */
                uint64_t v = (uint64_t)r16(src) | (uint64_t)r16(src + 2) << 16;
                memcpy(dst, &v, 8);
            } else {
                if (k == 0 && report)
                    w16_report("initial data: %u:%#x not converted (%u x %u bytes, %u here)", g->seg, g->off, g->count,
                               g->w16, g->wn);
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
            if (g->seg == fix[i].seg && fix[i].off >= g->off && fix[i].off + 4 <= g->off + span &&
                (g->w16 == g->wn || (g->w16 == 4 && g->wn == sizeof(void *)))) {
                void *p = map_far(fix[i].tseg, fix[i].toff);
                /* the same element here: at its offset, or a pointer element of its own width */
                size_t at = g->w16 == g->wn ? fix[i].off - g->off : (size_t)(fix[i].off - g->off) / 4 * g->wn;
                if (!p)
                    unmapped++;
                memcpy((char *)g->addr + at, &p, sizeof p);
                break;
            }
        }
    }
    w16_trace("initial data: %u globals (src/gen/exe_data.c), %u relocations (%u to no function)", loaded, nfix, unmapped);
    w16_initial_loaded = (int)loaded;
    (void)fixup_at;
}
