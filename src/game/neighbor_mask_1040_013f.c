/* neighbor_mask_1040_013f -- shipped at 1040:013f.
 *
 * Ported by tools/port.py from matched/game/neighbor_mask_1040_013f.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 tile_passable_1040_00d5(col_i16 p, col_i16 d);

col_i16 neighbor_mask_1040_013f(col_i16 p, col_i16 unused1)
{
    register col_i16 m = 0;

    if (tile_passable_1040_00d5(p, -(*g_map_width)) != 0)
        m += 8;
    if (tile_passable_1040_00d5(p, (*g_map_width)) != 0)
        m += 4;
    if (tile_passable_1040_00d5(p, -1) != 0)
        m += 2;
    if (tile_passable_1040_00d5(p, 1) != 0)
        m++;
    return m;
}
