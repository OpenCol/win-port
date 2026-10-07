/* PORT: this file's own declarations of win_set_cursor_1078_2986 removed: the header declares it from the definition (W1 step 3). */
/* MapWin_TrackMouse: candidate for 1038:9b39 (584 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_TrackMouse
 * name-source: sibling-build
 *
 * The pointer moving over the map window sets the cursor. In view mode (1) the cursor is reset
 * and the drag state (+0x784) cleared. Otherwise the pointer is converted to a map square;
 * on the selected unit's square the cursor for the returned direction (table at
 * SEG20:0x7746, 4 bytes each; 0 = default) is installed and remembered in DGROUP:0x1c54 as
 * 0x1fe + direction and in +0x784; off the unit, +0x786 set with DGROUP:0x1c4e non-zero
 * gives the hand cursor (0x80, state -2), otherwise the default cursor (0x1f4, state -1).
 *
 * NAME SWAPPED 2026-09-26 with `MapWin_LeftMouseDown`. This function is installed on handler slot +0, and the
 * shipped object_message_handler dispatches that slot on WM_MOUSEMOVE. Finding 59 had attached the
 * Mac names by slot from the premise that +4 is the mouse-move slot; the compare chain says +0
 * is. The description below is of this body; only its framing words were changed.
 *
 * RENAMED 2026-09-26 from mapview_pointer_at: the Mac build's MapWin::TrackMouse(short, short): MapWin_Init installs it as the mousemove handler, and it takes the same two shorts. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern void *GetEventWindow(void);
extern int MapWin_WinPos2MapPos(MapWin *o, int x, int y, int *mx, int *my);
extern void MSSetCursor_id(MSWin *win, int cursor);

/* Two same-module helpers are defined here because the shipped calls to them are the
 * compiler's own `push cs / call `: only a callee defined in the translation unit gets
 * that form. They pass the window's two handles (+6, +8) on to the cursor setters. */
extern "C" void win_load_cursor(DialogWin *w, int cursor);

extern "C" void win_set_cursor_1038_e30a(DialogWin *w, CursorClass *cursor);

void MapWin_TrackMouse(int x, int y)
{
    MapWin *o;
    int mx, my;
    EventWin *w = (EventWin *)GetEventWindow();
    int r;

    o = (MapWin *)w;
    if (g_game.view_mode == 1) {
        win_load_cursor((DialogWin *)(&(DialogWin &)*o), -1);
        o->drag = -1;
        return;
    }
    if (o != 0 && g_cursor_state != 0x80) {
        x -= o->rect.left;
        y -= o->rect.top;
        r = MapWin_WinPos2MapPos(o, x, y, &mx, &my);
        if (o->lbutton_held != 0) {
            if (g_units[g_game.selected_unit].x == mx && g_units[g_game.selected_unit].y == my) {
                if (r + 0x1fe != g_cursor_state) {
                    if (r == 0)
                        win_load_cursor((DialogWin *)(&(DialogWin &)*o), -1);
                    else
                        win_set_cursor_1038_e30a(&(DialogWin &)*o, &g_cursors[r]);
                    g_cursor_state = r + 0x1fe;
                    o->drag = r;
                }
            } else if (g_press_on_unit != 0) {
                if (g_cursor_state != 0x80) {
                    win_set_cursor_1038_e30a(&(DialogWin &)*o, &g_cursors[0]);
                    g_cursor_state = 0x80;
                    o->drag = -2;
                }
            } else
                ;
        } else {
            if (g_units[g_game.selected_unit].x == mx && g_units[g_game.selected_unit].y == my) {
                if (r + 0x1fe != g_cursor_state) {
                    if (r == 0)
                        win_load_cursor((DialogWin *)(&(DialogWin &)*o), -1);
                    else
                        win_set_cursor_1038_e30a(&(DialogWin &)*o, &g_cursors[r]);
                    g_cursor_state = r + 0x1fe;
                    o->drag = r;
                }
            } else if (g_cursor_state != 0x1f4) {
                win_load_cursor((DialogWin *)(&(DialogWin &)*o), -1);
                g_cursor_state = 0x1f4;
                o->drag = -1;
            }
        }
    }
}
