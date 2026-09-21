/* orthogonal_neighbor_bits -- shipped at 1038:d68c.
 *
 * Ported by tools/port.py from matched/game/orthogonal_neighbor_bits.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char huge *g_map_tiles;              DGROUP:0x4c72
 *   extern int g_map_width;                              DGROUP:0x4c40
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 orthogonal_neighbor_bits(col_i16 mask, col_i16 unused1)
{
    col_i16 r = 0;

    if (mask & *(g_map_tiles - (*g_map_width))) r += 8;
    if (mask & *(g_map_tiles + (*g_map_width))) r += 4;
    if (mask & *(g_map_tiles - 1)) r += 2;
    if (mask & *(g_map_tiles + 1)) r += 1;
    return r;
}
