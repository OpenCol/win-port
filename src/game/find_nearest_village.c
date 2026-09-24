/* find_nearest_village -- shipped at 1038:05b8.
 *
 * Ported by tools/port.py from matched/game/find_nearest_village.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Settlement far g_settlements[];               SEG20:0xcb9a
 *   extern int far g_settlement_count;                   SEG20:0x779c
 *   extern int far g_nearest_dist;                       DGROUP:0x4bc6
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void set_current_native_settlement(col_i16 v);

typedef struct { unsigned char x, y, owner; char rest[15]; } Settlement_local; /* 18 bytes at SEG20:0xcb9a */




extern col_i16 square_region_if_land(col_i16 x, col_i16 y);
extern col_i16 octile_distance(col_i16 dx, col_i16 dy);



col_i16 find_nearest_village(col_i16 x, col_i16 y, col_i16 nation, col_i16 land)
{
    register col_i16 v;
    register col_i16 d;
    col_i16 best = -1;
    col_i16 bestd = 9999;
    col_i16 nib;

    for (v = 0; v < g_settlement_count; v++) {
        if (nation >= 0 && g_settlements[v].owner != (unsigned char)nation)
            continue;
        if (land >= 0) {
            nib = square_region_if_land(g_settlements[v].x, g_settlements[v].y);
            if (nib != land)
                continue;
        }
        d = octile_distance(x - g_settlements[v].x, y - g_settlements[v].y);
        if (d <= bestd) {
            best = v;
            bestd = d;
        }
    }
    g_nearest_dist = bestd;
    if (best >= 0)
        set_current_native_settlement(best);
    return best;
}
