/* tile_passable_1038_d44f -- shipped at 1038:d44f.
 *
 * Ported by tools/port.py from matched/game/tile_passable_1038_d44f.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char huge *g_map_tiles;              DGROUP:0x4c72
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 tile_passable_1038_d44f(col_i16 p, col_i16 d)
{
    register col_i16 r = 0;
    col_i16 t;

    t = g_map_tiles[d];
    t = (unsigned char)(t & 0x1f);
    if (t >= 0x18)
        goto done;
    if ((t & 7) == 1)
        goto done;
    if (t > 7)
        r = 1;
done:
    return r;
}
