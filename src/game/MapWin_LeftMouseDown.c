/* PORT: this file's own declarations of win_set_cursor_1038_e30a, win_show_and_focus_1038_e279 removed: the header declares them from the definition (W1 step 3). */
/* MapWin_LeftMouseDown: candidate for 1038:963d (426 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_LeftMouseDown
 *
 * A left button press on the map window (re-entry byte at DGROUP:0x1c56): outside the map
 * rectangle the window is handled by win_show_and_focus_1038_e279; inside, the map position is computed,
 * +0x786 set, DGROUP:0x1c4e records whether the selected unit stands there, it waits 5
 * ticks and -- when the window accepts (Screen_LButtonDownNow) and the view mode is not 1 --
 * 20 ticks more; if the window still accepts and the hover state (0x1c54) is not yet 0x80, the
 * the default CURSOR at SEG20:0x7746 is installed, the state set, +0x784 set to -2 and the
 * window handled. This was read as hover text; `win_set_cursor_1038_e30a` is the same call
 * `MapWin_TrackMouse` makes to install a cursor from that very table.
 *
 * NAME SWAPPED 2026-09-26 with `MapWin_TrackMouse`. This function is installed on handler slot +4, and the
 * shipped object_message_handler dispatches that slot on WM_LBUTTONDOWN. Finding 59 had attached the
 * Mac names by slot from the premise that +4 is the mouse-move slot; the compare chain says +0
 * is. The description below is of this body; only its framing words were changed.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern void *GetEventWindow(void);

extern GameState g_game;   /* SEG20:0x7782 */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int MapWin_WinPos2MapPos(MapWin *o, int x, int y, int *mx, int *my);

extern int Screen_LButtonDownNow(void *w);

void MapWin_LeftMouseDown(int x, int y)
{
    MapWin *o;
    int mx;
    int my;
    EventWin *w;

    if (g_map_ldown_busy != 0)
        return;
    g_map_ldown_busy = 1;
    w = (EventWin *)GetEventWindow();
    o = (MapWin *)w;
    if (o->rect.left > x || o->rect.right < x || o->rect.top > y || o->rect.bottom < y) {
        win_show_and_focus_1038_e279(&(DialogWin &)*o);
    } else {
        x -= o->rect.left;
        y -= o->rect.top;
        MapWin_WinPos2MapPos(o, x, y, &mx, &my);
        o->lbutton_held = 1;
        if (g_units[g_game.selected_unit].x == mx && g_units[g_game.selected_unit].y == my)
            g_press_on_unit = 1;
        else
            g_press_on_unit = 0;
        MSWaitTicks_1038_e2f1(5);
        if (Screen_LButtonDownNow(&(DialogWin &)*o) != 0 && g_game.view_mode != 1) {
            MSWaitTicks_1038_e2f1(0x14);
            if (Screen_LButtonDownNow(&(DialogWin &)*o) != 0 && g_cursor_state != 0x80) {
                win_set_cursor_1038_e30a(&(DialogWin &)*o, g_cursors);
                g_cursor_state = 0x80;
                o->drag = -2;
                win_show_and_focus_1038_e279(&(DialogWin &)*o);
            }
        }
    }
    MSDoAllTasks_1038_e2a2();
    g_map_ldown_busy = 0;
}
