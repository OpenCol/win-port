/* SetGlobalSpriteScale: candidate for 1068:6420 (456 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: SetGlobalSpriteScale
 *
 * Selects the scaling map for a scale RATIO num/den (callers pass 1,1 to reset, 3,4 and 2,1), from a sixteen-slot cache of
 * 0x808-byte entries in segment 33. A slot whose stored ratio matches is re-stamped and
 * used; otherwise the least recently stamped slot is taken and its 1024-entry map rebuilt
 * by Bresenham, each entry naming the source pixel a destination pixel reads. The chosen
 * map is left in the global at DGROUP:0x5694 for the blitter to use.
 *
 * 456 bytes over 89 call sites, the most-called function of its size in the binary.
 *
 * RENAMED 2026-09-26 from scale_map_select: the Mac build's SetGlobalSpriteScale(short, short), in the Mac's Sprite scaling module beside InitScaleTable: (w, h) on both sides, called by EconReport3, the PediaWin article pages and the Draw* map and colony methods as this is, and its one callee is MSGetTicks -- the clock this function's least-recently-stamped cache of scale maps needs.
 */

#include "colonization.h"

#include "records.h"
extern ScaleSlot g_scale_cache[];   /* SEG33:0x0000 -- sixteen 0x808-byte slots, the whole 0x8080-byte segment */

/* same object: 4-byte `push cs; call ` (rule 47) */

void SetGlobalSpriteScale(int num, int den)
{
    int step;
    int best;
    long oldest;
    register int i;
    register int acc;

    g_scale_num = max_int_1068_98e4(num, 1);
    g_scale_den = max_int_1068_98e4(den, 1);
    oldest = MSGetTicks_1068_990a();
    best = -1;
    for (i = 0; i < 16; i++) {
        if (g_scale_cache[i].num == g_scale_num && g_scale_cache[i].den == g_scale_den) {
            g_scale_cache[i].stamp = MSGetTicks_1068_990a();
            g_scale_map = g_scale_cache[i].map;
            return;
        }
        if (g_scale_cache[i].stamp <= oldest) {
            oldest = g_scale_cache[i].stamp;
            best = i;
        }
    }
    g_scale_cache[best].num = g_scale_num;
    g_scale_cache[best].den = g_scale_den;
    g_scale_cache[best].stamp = MSGetTicks_1068_990a();
    step = 0;
    if (g_scale_num >= g_scale_den) {
        acc = g_scale_num;
        i = 0;
        while (i < 0x400) {
            while (acc >= g_scale_den && i < 0x400) {
                g_scale_cache[best].map[i] = step;
                i++;
                acc -= g_scale_den;
            }
            acc += g_scale_num;
            step++;
        }
    } else {
        acc = 0;
        i = 0;
        while (i < 0x400) {
            while (acc >= 0 && i < 0x400) {
                g_scale_cache[best].map[i] = step;
                i++;
                acc -= g_scale_den;
            }
            acc += g_scale_num;
            step++;
        }
    }
    g_scale_map = g_scale_cache[best].map;
}
