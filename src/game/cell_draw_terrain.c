/* cell_draw_terrain -- shipped at 1008:7e2d.
 *
 * Ported by tools/port.py from matched/game/cell_draw_terrain.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char huge *g_cur_cell;               DGROUP:0x4b94
 *   extern unsigned char g_cell_terrain;                 DGROUP:0x4b8f
 *   extern unsigned char g_cell_class;                   DGROUP:0x4b8e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 coast_neighbours(void);

extern unsigned char * map_plane0_ptr(col_i16 x, col_i16 y);
extern char terrain_class(col_i16 t);

/* same object: 4-byte `push cs; call ` */


char cell_draw_terrain(col_i16 x, col_i16 y)
{
    char coastal;
    char save;
    col_i16 r;
    register col_i16 n;

    n = 0;
    r = -1;
    g_cur_cell = map_plane0_ptr(x, y);
    g_cell_terrain = *g_cur_cell;
    g_cell_class = terrain_class(g_cell_terrain);
    if (g_cell_class == 0x19 || g_cell_class == 0x1a) {
        save = g_cell_class;
        n = coast_neighbours();
        coastal = 1;
    }
    if (coastal == 0 || n != 0) {
        if (g_cell_class < 0x18)
            r = g_cell_class & 7;
        else
            r = g_cell_class;
    }
    return r;
}
