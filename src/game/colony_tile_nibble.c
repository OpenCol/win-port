/* colony_tile_nibble -- shipped at 1018:3c4b.
 *
 * Ported by tools/port.py from matched/game/colony_tile_nibble.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0xca bytes */

extern col_i16 map_plane2_nibble_if_land(col_i16 x, col_i16 y);

col_i16 colony_tile_nibble(col_i16 c)
{
    return map_plane2_nibble_if_land(g_colonies[c].x, g_colonies[c].y);
}
