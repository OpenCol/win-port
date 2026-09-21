/* tile_neighbor_mask -- shipped at 1040:0446.
 *
 * Ported by tools/port.py from matched/game/tile_neighbor_mask.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern unsigned char huge *g_map_tiles2;             DGROUP:0x4c82
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 tile_neighbor_mask(col_i16 mask, col_i16 unused1)
{
    col_i16 r = 0;

    if (*(g_map_tiles2 - (*g_map_width)) & mask) r += 8;
    if (*(g_map_tiles2 + (*g_map_width)) & mask) r += 4;
    if (*(g_map_tiles2 - 1) & mask) r += 2;
    if (*(g_map_tiles2 + 1) & mask) r += 1;
    return r;
}
