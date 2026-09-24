/* mapview_pointer_at -- shipped at 1038:963d.
 *
 * Ported by tools/port.py from matched/game/mapview_pointer_at.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern char g_map_hover_busy;                        DGROUP:0x1c56
 *   extern int g_hover_on_unit;                          DGROUP:0x1c4e
 *   extern int g_cursor_state;                           DGROUP:0x1c54
 *   extern int far g_view_mode;                          SEG20:0x7792
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern char far g_cursors[];                         SEG20:0x7746
 *
 * The file's own Unit, MapWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSDoAllTasks_1038_e2a2(void);
void MSWaitTicks_1038_e2f1(col_i16 cursor);
void close_dialog_1038_e279(void *w);
void win_set_cursor_1038_e30a(void *w, void *text);

extern char * current_window(void);






/* same object: 4-byte `push cs; call ` (rule 47) */

extern void MapWin_WinPos2MapPos(MapWin *o, col_i16 x, col_i16 y, col_i16 *mx, col_i16 *my);

extern col_i16 mouse_left_down_1060_a819(void *w);


void mapview_pointer_at(col_i16 x, col_i16 y)
{
    MapWin *o;
    col_i16 mx;
    col_i16 my;
    char *w;

    if (g_map_hover_busy != 0)
        return;
    g_map_hover_busy = 1;
    w = current_window();
    o = (MapWin *)(w != 0 ? w - 0x26 : w);
    if (o->ox > x || o->right < x || o->oy > y || o->bottom < y) {
        close_dialog_1038_e279((char *)o + 0x26);
    } else {
        x -= o->ox;
        y -= o->oy;
        MapWin_WinPos2MapPos(o, x, y, &mx, &my);
        o->scroll = 1;
        if (g_units[g_selected_unit].x == mx && g_units[g_selected_unit].y == my)
            g_hover_on_unit = 1;
        else
            g_hover_on_unit = 0;
        MSWaitTicks_1038_e2f1(5);
        if (mouse_left_down_1060_a819((char *)o + 0x26) != 0 && g_view_mode != 1) {
            MSWaitTicks_1038_e2f1(0x14);
            if (mouse_left_down_1060_a819((char *)o + 0x26) != 0 && g_cursor_state != 0x80) {
                win_set_cursor_1038_e30a((char *)o + 0x26, ((col_i16 *)g_cursors));
                g_cursor_state = 0x80;
                o->drag = -2;
                close_dialog_1038_e279((char *)o + 0x26);
            }
        }
    }
    MSDoAllTasks_1038_e2a2();
    g_map_hover_busy = 0;
}
