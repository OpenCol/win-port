/* MapWin_WinPos2MapPos -- shipped at 1038:a0f2.
 *
 * Ported by tools/port.py from matched/game/MapWin_WinPos2MapPos.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_view_x0;                            DGROUP:0x4c44
 *   extern int far g_view_y0;                            DGROUP:0x4c46
 *
 * The file's own MapWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 MapWin_TileSize(MapWin *w);

col_i16 MapWin_WinPos2MapPos(MapWin *o, col_i16 x, col_i16 y, col_i16 *mx, col_i16 *my)
{
    col_i16 third;
    col_i16 fx;
    col_i16 cx;
    col_i16 cy;
    col_i16 fy;
    col_i16 t;

    t = MapWin_TileSize(o);
    cx = x / t;
    cy = y / t;
    *mx = g_view_x0 + cx;
    *my = g_view_y0 + cy;
    third = t / 3;
    fx = (x - cx * t) / third;
    fy = (y - cy * t) / third;
    if (fx == 0) {
        if (fy == 0)
            return 8;
        else if (fy == 1)
            return 7;
        else
            return 6;
    } else if (fx == 1) {
        if (fy == 0)
            return 1;
        else if (fy == 1)
            return 0;
        else
            return 5;
    } else {
        if (fy == 0)
            return 2;
        else if (fy == 1)
            return 3;
        else
            return 4;
    }
}
