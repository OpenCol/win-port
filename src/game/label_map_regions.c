/* label_map_regions -- shipped at 1038:c2a9.
 *
 * Ported by tools/port.py from matched/game/label_map_regions.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short g_plane2_handle;                        DGROUP:0x1c6e
 *   extern short g_plane3_handle;                        DGROUP:0x1c70
 *   extern unsigned char huge *g_plane2;                 DGROUP:0x1c7a
 *   extern unsigned char huge *g_plane3;                 DGROUP:0x1c7e
 *   extern int g_map_width;                              DGROUP:0x4c40
 *   extern int g_map_h;                                  DGROUP:0x4c42
 *   extern int far g_region_size[];                      SEG27:0x008c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1038_e46c(col_i16 a, col_i16 b);
col_i16 min_int_1038_e446(col_i16 a, col_i16 b);

extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_u16 h);
extern void * memset_rtl(void *p, col_i16 c, col_u16 n);
extern void handle_fill_bytes(col_i16 handle, char c);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 on_map(col_i16 x, col_i16 y);
extern void check_timer_tick(col_i16 n);

/* same object: 4-byte `push cs; call ` (rule 47) */



col_i16 label_map_regions(void)
{
    col_i16 flags;
    col_i16 x;
    col_i16 y;
    col_i16 k;
    col_i16 pass;
    col_i16 next_id;
    col_i16 cur;
    col_i16 t;
    col_i16 placed;
    col_i16 d;
    col_i16 lo;
    col_i16 hi;
    col_i16 i;
    unsigned char *p;
    unsigned char *q;
    col_u16 *counts;
    col_i16 h;
    register col_i16 j;

    flags = 0;
    counts = 0;
    h = MSAllocateHandle(0x4000L);
    counts = (col_u16 *)MSHLock(h);
    handle_fill_bytes(g_plane2_handle, 0);
    for (pass = 1; pass >= 0; pass--) {
        memset_rtl(counts, 0, 0x4000);
        handle_fill_bytes(g_plane3_handle, 0);
        next_id = 0;
        cur = 0;
        placed = 0;
        for (y = 1; g_map_h - 1 > y; y++) {
            for (x = (*g_map_width) - 2; x >= 1; x--) {
                if (on_map(x, y) == 0 || ocean(x, y) != pass) {
                    placed = 0;
                    cur = 0;
                    continue;
                }
                for (d = -1; d <= 1; d++) {
                    t = *(g_plane3 + (x + d) + (y - 1) * (*g_map_width));
                    if (t != 0) {
                        lo = t;
                        if (cur != 0 && cur != t) {
                            lo = min_int_1038_e446(t, cur);
                            hi = max_int_1038_e46c(t, cur);
                            counts[lo] += counts[lo];
                            counts[hi] = 0;
                            for (k = 1; k <= y; k++)
                                for (j = 1; j <= (*g_map_width); j++) {
                                    p = g_plane3 + j + k * (*g_map_width);
                                    if (*p == (unsigned char)hi)
                                        *p = (unsigned char)lo;
                                }
                        }
                        cur = t = lo;
                    }
                }
                if (cur == 0) {
                    if (placed == 0) {
                        next_id = 0;
                        if ((y == 1 || g_map_h - 2 == y) && pass == 0)
                            next_id = 0x10;
                        while (counts[++next_id] != 0)
                            ;
                        if (next_id >= 0x2000)
                            next_id = 0x1fff;
                    }
                    cur = next_id;
                }
                p = g_plane3 + x + y * (*g_map_width);
                *p = (unsigned char)cur;
                counts[cur]++;
                placed = 1;
            }
            check_timer_tick(0);
        }
        p = g_plane3;
        q = g_plane2;
        for (y = 0; y < g_map_h; y++) {
            for (x = 0; x < (*g_map_width); x++) {
                if (*p != 0) {
                    if (*p > 0xf) {
                        if (counts[*p] <= 0) {
                            *p = (unsigned char)(counts[*p] > 0 ? counts[*p] : ~counts[*p] + 1);
                        } else {
                            next_id = 0;
                            while (counts[++next_id] != 0)
                                ;
                            if (next_id <= 0xf) {
                                counts[next_id] = counts[*p];
                                counts[*p] = -next_id;
                                *p = (unsigned char)next_id;
                            } else {
                                flags |= 1 << pass;
                                *p = 0xf;
                            }
                        }
                    }
                    *q = *p;
                }
                p++;
                q++;
            }
            check_timer_tick(0);
        }
    }
    for (i = 0; i < 0x10; i++)
        g_region_size[i] = counts[i];
    counts = (col_u16 *)MSHUnlock(h);
    MSDisposeHandle(h);
    return flags;
}
