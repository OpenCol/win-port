/* MapWin_LeftMouseDown -- shipped at 1038:9b39.
 *
 * Ported by tools/port.py from matched/game/MapWin_LeftMouseDown.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_cursor_state;                           DGROUP:0x1c54
 *   extern int g_hover_on_unit;                          DGROUP:0x1c4e
 *   extern int far g_view_mode;                          SEG20:0x7792
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern int far g_cursors[][2];                       SEG20:0x7746
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own MapWin, Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void win_load_cursor_1038_e331(void *w, col_i16 cursor);
void win_set_cursor_1038_e30a(void *w, void *cursor);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

extern char * current_window(void);
extern col_i16 MapWin_WinPos2MapPos(MapWin *o, col_i16 x, col_i16 y, col_i16 *mx, col_i16 *my);
typedef struct { char pad[6]; void *win; } GameWinBase;
extern void win_load_cursor_1078_29b6(void *w, col_i16 cursor);
extern void win_set_cursor_1078_2986(void *w, col_i16 cursor);

/* Two same-module helpers are defined here because the shipped calls to them are the
 * compiler's own `push cs / call `: only a callee defined in the translation unit gets
 * that form. They pass the window's two handles (+6, +8) on to the cursor setters. */




void MapWin_LeftMouseDown(col_i16 x, col_i16 y)
{
    MapWin *o;
    col_i16 mx, my;
    char *w = current_window();
    col_i16 r;

    o = (MapWin *)(w != 0 ? w - 0x26 : w);
    if (g_view_mode == 1) {
        win_load_cursor_1038_e331((GameWinBase *)((char *)o + 0x26), -1);
        o->drag = -1;
        return;
    }
    if (o != 0 && g_cursor_state != 0x80) {
        x -= o->ox;
        y -= o->oy;
        r = MapWin_WinPos2MapPos(o, x, y, &mx, &my);
        if (o->scroll != 0) {
            if (g_units[g_selected_unit].x == mx && g_units[g_selected_unit].y == my) {
                if (r + 0x1fe != g_cursor_state) {
                    if (r == 0)
                        win_load_cursor_1038_e331((GameWinBase *)((char *)o + 0x26), -1);
                    else
                        win_set_cursor_1038_e30a((GameWinBase *)((char *)o + 0x26), g_cursors[r]);
                    g_cursor_state = r + 0x1fe;
                    o->drag = r;
                }
            } else if (g_hover_on_unit != 0) {
                if (g_cursor_state != 0x80) {
                    win_set_cursor_1038_e30a((GameWinBase *)((char *)o + 0x26), g_cursors[0]);
                    g_cursor_state = 0x80;
                    o->drag = -2;
                }
            } else
                ;
        } else {
            if (g_units[g_selected_unit].x == mx && g_units[g_selected_unit].y == my) {
                if (r + 0x1fe != g_cursor_state) {
                    if (r == 0)
                        win_load_cursor_1038_e331((GameWinBase *)((char *)o + 0x26), -1);
                    else
                        win_set_cursor_1038_e30a((GameWinBase *)((char *)o + 0x26), g_cursors[r]);
                    g_cursor_state = r + 0x1fe;
                    o->drag = r;
                }
            } else if (g_cursor_state != 0x1f4) {
                win_load_cursor_1038_e331((GameWinBase *)((char *)o + 0x26), -1);
                g_cursor_state = 0x1f4;
                o->drag = -1;
            }
        }
    }
}
