/* coast_neighbours -- shipped at 1008:7cb5.
 *
 * Ported by tools/port.py from matched/game/coast_neighbours.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char huge *g_cur_cell;               DGROUP:0x4b94
 *   extern unsigned char g_coast_edges[4];               DGROUP:0x4b90
 *   extern unsigned char g_cell_terrain;                 DGROUP:0x4b8f
 *   extern unsigned char g_cell_class;                   DGROUP:0x4b8e
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "terrain.h"







extern char terrain_class(col_i16 t);

col_i16 coast_neighbours(void)
{
    col_i16 d;
    col_i16 h;
    unsigned char t;
    char cls;
    col_i16 count;
    col_i16 mask;
    register col_i16 a;
    register col_i16 b;

    count = 0;
    mask = 0;
    for (d = 0; d < 4; d++)
        g_coast_edges[d] = 0;
    for (d = 0; d < 8; d++) {
        t = *(g_cur_cell + g_dir_dx[d] + ((col_i16)g_dir_dy[d] < 0 ? -(*g_map_width) : 0)
                                       + ((col_i16)g_dir_dy[d] > 0 ? (*g_map_width) : 0));
        t = t & 0x1f;
        t = t < 0x18 ? t & 7 : t;
        cls = terrain_class(t);
        if (cls != TERRAIN_OCEAN && cls != TERRAIN_SEA_LANE) {
            mask |= (unsigned char)(1 << d);
            count++;
            if (d & 1) {
                a = ((d + 1) & 7) >> 1;
                g_coast_edges[a] |= 2;
            } else {
                g_cell_terrain = t;
                h = d >> 1;
                a = h;
                b = (h + 1) & 3;
                g_coast_edges[a] |= 4;
                g_coast_edges[b] |= 1;
            }
        }
    }
    g_cell_class = terrain_class(g_cell_terrain);
    return count;
}
