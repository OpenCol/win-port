/* unit_place_at -- shipped at 1038:1ca8.
 *
 * Ported by tools/port.py from matched/game/unit_place_at.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int g_stub_a;                                 
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_at(col_i16 x, col_i16 y);

#include "terrain.h"
#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */


extern col_i16 on_map(col_i16 x, col_i16 y);
extern void map_plane1_set_bits(col_i16 x, col_i16 y, col_i16 mask, col_i16 on);
extern col_i16 tile_is_river_mouth(col_i16 x, col_i16 y);
extern void map_plane2_set_high_nibble(col_i16 x, col_i16 y, col_i16 v);



void unit_place_at(col_i16 u, col_i16 x, col_i16 y)
{
    col_i16 f;

    f = unit_at(x, y);
    g_units[u].x = x;
    g_units[u].y = y;
    g_units[u].link = -1;
    g_units[u].link2 = f;
    if (f >= 0) {
        g_units[f].link = u;
    } else if (on_map(x, y) != 0) {
        map_plane1_set_bits(x, y, MAP1_UNIT, 1);
        if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) < 4 || tile_is_river_mouth(x, y) == 0)
            map_plane2_set_high_nibble(x, y, (unsigned char)(g_units[u].flags & UNIT_NATION_MASK));
    }
}
