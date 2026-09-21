/* tile_passable_1040_00d5 -- shipped at 1040:00d5.
 *
 * Ported by tools/port.py from matched/game/tile_passable_1040_00d5.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char huge *g_map_tiles2;             DGROUP:0x4c82
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 tile_passable_1040_00d5(col_i16 p, col_i16 d)
{
    register col_i16 r = 0;
    col_i16 t;

    t = g_map_tiles2[d];
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
