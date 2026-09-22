/* unit_tile_nibble -- shipped at 1038:2f99.
 *
 * Ported by tools/port.py from matched/game/unit_tile_nibble.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 map_plane2_nibble_if_land(col_i16 x, col_i16 y);

col_i16 unit_tile_nibble(register col_i16 u)
{
    return map_plane2_nibble_if_land(g_units[u].x, g_units[u].y);
}
