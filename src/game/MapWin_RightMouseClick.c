/* MapWin_RightMouseClick -- shipped at 1038:950c.
 *
 * Ported by tools/port.py from matched/game/MapWin_RightMouseClick.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int g_map_click_busy;                         DGROUP:0x1c52
 *   extern int far g_view_mode;                          SEG20:0x7792
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern char far g_menu_owner;                        SEG20:0x0656
 *
 * The file's own Unit, MapWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSDoAllTasks_1038_e2a2(void);
void close_dialog_1038_e279(void *w);
void menu_add_item(void *w, void *menu, col_i16 kind, col_i16 x, col_i16 y);

extern char * current_window(void);




/* same object: 4-byte `push cs; call ` (rule 47) */

extern void MapWin_WinPos2MapPos(MapWin *o, col_i16 x, col_i16 y, col_i16 *mx, col_i16 *my);


void MapWin_RightMouseClick(col_i16 x, col_i16 y)
{
    MapWin *o;
    col_i16 mx;
    col_i16 my;
    char *w;

    if (g_map_click_busy != 0)
        return;
    g_map_click_busy = 1;
    w = current_window();
    o = (MapWin *)(w != 0 ? w - 0x26 : w);
    if (o->ox > x || o->right < x || o->oy > y || o->bottom < y) {
        close_dialog_1038_e279((char *)o + 0x26);
    } else {
        x -= o->ox;
        y -= o->oy;
        MapWin_WinPos2MapPos(o, x, y, &mx, &my);
        if (g_view_mode == 0 && g_units[g_selected_unit].x == mx && g_units[g_selected_unit].y == my)
            menu_add_item((char *)o + 0x26, &g_menu_owner, 2, x, y);
    }
    MSDoAllTasks_1038_e2a2();
    g_map_click_busy = 0;
}
