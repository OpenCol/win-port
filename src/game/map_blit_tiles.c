/* map_blit_tiles -- shipped at 1040:07f1.
 *
 * Ported by tools/port.py from matched/game/map_blit_tiles.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_view_x0;                            DGROUP:0x4c44
 *   extern int far g_view_y0;                            DGROUP:0x4c46
 *   extern int far g_tile_w;                             DGROUP:0x4c4c
 *   extern int far g_tile_h;                             DGROUP:0x4c4e
 *   extern char far g_map_window[];                      SEG20:0x0658
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void map_blit_rect(void *w, col_i16 x, col_i16 y, col_i16 wd, col_i16 ht);

void map_blit_tiles(col_i16 x, col_i16 y, col_i16 w, col_i16 h)
{
    col_i16 px;
    col_i16 py;
    register col_i16 pw;
    register col_i16 ph;

    px = (x - g_view_x0) * g_tile_w;
    py = (y - g_view_y0) * g_tile_h;
    pw = g_tile_w * w;
    ph = g_tile_h * h;
    if (pw > 0 && ph > 0)
        map_blit_rect((&g_map_window), px, py, pw, ph);
}
