/* tile_decoration -- shipped at 1038:c066.
 *
 * Ported by tools/port.py from matched/game/tile_decoration.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_scenery_seed;                           DGROUP:0x1ca6
 *   extern int far g_terrain_bonus[];                    SEG27:0x02ca
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 map_index_in_range(col_i16 x, col_i16 y);
col_i16 square_native_nation(col_i16 x, col_i16 y);
unsigned char map_plane0_at(col_i16 x, col_i16 y);
unsigned char map_plane1_at(col_i16 x, col_i16 y);

#include "terrain.h"



extern col_i16 terrain_at(col_i16 x, col_i16 y);






col_i16 tile_decoration(col_i16 x, col_i16 y)
{
    col_i16 r = -1;
    col_i16 v;
    col_i16 h;
    col_i16 cls;
    col_i16 t;

    if (g_scenery_seed == 0)
        goto done;
    if (map_index_in_range(x, y) == 0)
        return -1;
    if (square_native_nation(x, y) >= 0)
        goto done;
    t = map_plane0_at(x, y);
    t &= 0xff3f;
    cls = (t >= 8 && t < 0x10) || (t >= 0x10 && t < 0x18);
    v = (x >> 2) * 17 + g_scenery_seed + (y >> 2) * 19 + cls * 31;
    v = v & 0xf;
    h = ((x & 3) << 2) + (y & 3);
    if (h != v && (v ^ 0xa) != h)
        goto done;
    r = g_terrain_bonus[terrain_at(x, y)];
    if (r == 0)
        r = 6;
    if (map_plane1_at(x, y) & MAP1_DEPLETED) {
        if (r == 12)
            r = 0;
        else
            r = -1;
    }
done:
    return r;
}
