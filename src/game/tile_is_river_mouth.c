/* tile_is_river_mouth -- shipped at 1038:c193.
 *
 * Ported by tools/port.py from matched/game/tile_is_river_mouth.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_scenery_seed;                           DGROUP:0x1ca6
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char map_plane2_high_nibble(col_i16 x, col_i16 y);
col_i16 map_index_in_range(col_i16 x, col_i16 y);

#include "terrain.h"


extern col_i16 terrain_at(col_i16 x, col_i16 y);




col_i16 tile_is_river_mouth(col_i16 x, col_i16 y)
{
    col_i16 r = 0;
    col_i16 v;
    col_i16 hi;
    col_i16 t;

    if (map_index_in_range(x, y) == 0)
        return 0;
    if (g_scenery_seed == 0)
        goto done;
    t = terrain_at(x, y);
    if (t == TERRAIN_OCEAN || t == TERRAIN_SEA_LANE)
        goto done;
    if (t == TERRAIN_ARCTIC)
        goto done;
    hi = map_plane2_high_nibble(x, y);
    if (hi >= 0)
        goto done;
    v = (x >> 2) * 17 + g_scenery_seed + (y >> 2) * 19 + 8;
    if (((x & 3) << 2) + (y & 3) != (v & 0x1f))
        goto done;
    r = 1;
done:
    return r;
}
