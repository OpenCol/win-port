/* unit_unlink -- shipped at 1038:1b8d.
 *
 * Ported by tools/port.py from matched/game/unit_unlink.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "terrain.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

extern col_i16 on_map(col_i16 x, col_i16 y);
extern void map_plane1_set_bits(col_i16 x, col_i16 y, col_i16 mask, col_i16 on);

void unit_unlink(col_i16 u)
{
    col_i16 linked = 0;

    if (g_units[u].link >= 0) {
        g_units[g_units[u].link].link2 = g_units[u].link2;
        linked = 1;
    }
    if (g_units[u].link2 >= 0) {
        g_units[g_units[u].link2].link = g_units[u].link;
        linked = 1;
    }
    if (linked == 0 && on_map(g_units[u].x, g_units[u].y) != 0)
        map_plane1_set_bits(g_units[u].x, g_units[u].y, MAP1_UNIT, 0);
    g_units[u].x = -1;
    g_units[u].y = -1;
}
