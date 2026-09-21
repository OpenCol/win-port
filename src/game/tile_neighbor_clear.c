/* tile_neighbor_clear -- shipped at 1040:01b2.
 *
 * Ported by tools/port.py from matched/game/tile_neighbor_clear.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern unsigned char huge *g_draw_p3;                DGROUP:0x4c86
 *   extern unsigned char g_draw_mask;                    DGROUP:0x4c92
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 tile_neighbor_clear(void)
{
    col_i16 r = 0;
    col_i16 f;

    f = g_draw_mask != 0 && (*(g_draw_p3 - (*g_map_width)) & g_draw_mask) == 0;
    if (f) r += 8;
    f = g_draw_mask != 0 && (*(g_draw_p3 + (*g_map_width)) & g_draw_mask) == 0;
    if (f) r += 4;
    f = g_draw_mask != 0 && (*(g_draw_p3 - 1) & g_draw_mask) == 0;
    if (f) r += 2;
    f = g_draw_mask != 0 && (*(g_draw_p3 + 1) & g_draw_mask) == 0;
    if (f) r += 1;
    return r;
}
