/* map_click_to_tile -- shipped at 1060:076c.
 *
 * Ported by tools/port.py from matched/game/map_click_to_tile.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_map_window;                        SEG20:0x0658
 *   extern int g_view_tile_w;                            DGROUP:0x4e0c
 *   extern int g_view_tile_h;                            DGROUP:0x4e0e
 *   extern int g_view_org_x;                             DGROUP:0x4e10
 *   extern int g_view_org_y;                             DGROUP:0x4e12
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void win_take_focus(void *w);

extern char * current_window(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void MapWin_Recenter(void *mw, col_i16 tx, col_i16 ty);






void map_click_to_tile(col_i16 x, col_i16 y)
{
    Win *c;
    col_i16 tx, ty;
    char *w = current_window();

    c = (Win *)(w != 0 ? w - 0x26 : w);
    if (c->left > x || c->right < x || c->top > y || c->bottom < y) {
        win_take_focus((char *)c + 0x26);
        return;
    }
    x -= c->left + g_view_org_x;
    y -= c->top + g_view_org_y;
    tx = x / g_view_tile_w;
    ty = y / g_view_tile_h;
    MapWin_Recenter(&g_map_window, tx, ty);
}
