/* unit_reaches_colony -- shipped at 1028:bd95.
 *
 * Ported by tools/port.py from matched/game/unit_reaches_colony.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern unsigned char far g_turn_nation;              SEG20:0x7796
 *   extern int far g_ocean_x;                            DGROUP:0x4bc8
 *   extern int far g_ocean_y;                            DGROUP:0x4bca
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *
 * The file's own Unit, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
extern void set_current_colony(col_i16 c);
extern col_i16 on_map(col_i16 x, col_i16 y);
extern unsigned char map_plane2_nibble(col_i16 x, col_i16 y);
extern col_i16 find_adjacent_ocean(col_i16 x, col_i16 y);

col_i16 unit_reaches_colony(col_i16 colony, col_i16 u)
{
    col_i16 r;

    r = 0;
    if (colony != 999) {
        set_current_colony(colony);
        if (g_cur_colony_rec->nation != g_turn_nation)
            goto out;
    }
    if (u >= 0) {
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
            col_i16 region;
            if (on_map(g_units[u].x, g_units[u].y) == 0)
                region = map_plane2_nibble((*g_map_width) - 2, 1);
            else if (find_adjacent_ocean(g_units[u].x, g_units[u].y) == 0)
                region = map_plane2_nibble(g_units[u].x, g_units[u].y);
            else
                region = map_plane2_nibble(g_ocean_x, g_ocean_y);
            if (colony != 999) {
                if (find_adjacent_ocean(g_cur_colony_rec->x,
                                                  g_cur_colony_rec->y) == 0)
                    goto out;
                if (map_plane2_nibble(g_ocean_x, g_ocean_y) != (unsigned char)region)
                    goto out;
            } else {
            }
        } else {
            col_i16 region = map_plane2_nibble(g_units[u].x, g_units[u].y);
            if (map_plane2_nibble(g_cur_colony_rec->x, g_cur_colony_rec->y) != (unsigned char)region)
                goto out;
        }
    }
    r = 1;
out:
    return r;
}
