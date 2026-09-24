/* map_draw_cursor -- shipped at 1038:aae0.
 *
 * Ported by tools/port.py from matched/game/map_draw_cursor.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_view_x0;                            DGROUP:0x4c44
 *   extern int far g_view_y0;                            DGROUP:0x4c46
 *   extern char far g_cursor_sprite[];                   SEG20:0x64a6
 *   extern int g_stub_a;                                 
 *
 * The file's own MapWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 MapWin_GetZoom(MapWin *w);
col_i16 MapWin_TileSize(MapWin *w);

extern void scale_map_select(col_i16 a, col_i16 b);
extern void sprite_draw(RECT *out, void *sprite, void *w, col_i16 x, col_i16 y);




void map_draw_cursor(MapWin *w, col_i16 x, col_i16 y)
{
    register col_i16 ts;
    col_i16 dx;
    col_i16 dy;
    col_i16 z;
    RECT r;

    ts = MapWin_TileSize(w);
    dx = (x - g_view_x0) * ts;
    dy = (y - g_view_y0) * ts;
    z = MapWin_GetZoom(w) + 8;
    scale_map_select(z, 8);
    sprite_draw(&r, g_cursor_sprite, w, w->ox + dx, w->oy + dy);
    scale_map_select(1, 1);
}
