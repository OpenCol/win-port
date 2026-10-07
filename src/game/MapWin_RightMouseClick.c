/* MapWin_RightMouseClick: candidate for 1038:950c (305 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_RightMouseClick
 *
 * A right click on the map window (re-entry flag at DGROUP:0x1c52): outside the map
 * rectangle (+0x76a..+0x770) the window is handled by win_show_and_focus_1038_e279; inside, the click is
 * converted to a map position and, in view mode 0 with the selected unit standing there,
 * track_popup_submenu_1038_e2b5 opens the unit's menu (kind 2). MSDoAllTasks_1038_e2a2 runs afterwards either way.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, MapWin, MenuClass removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-








#include "records.h"

extern void *GetEventWindow(void);

extern GameState g_game;   /* SEG20:0x7782 */


/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void win_show_and_focus_1038_e279(void *w);
extern int MapWin_WinPos2MapPos(MapWin *o, int x, int y, int *mx, int *my);
extern "C" void track_popup_submenu_1038_e2b5(void *w, void *menu, int kind, int x, int y);

void MapWin_RightMouseClick(int x, int y)
{
    MapWin *o;
    int mx;
    int my;
    EventWin *w;

    if (g_map_click_busy != 0)
        return;
    g_map_click_busy = 1;
    w = (EventWin *)GetEventWindow();
    o = (MapWin *)w;
    if (o->rect.left > x || o->rect.right < x || o->rect.top > y || o->rect.bottom < y) {
        win_show_and_focus_1038_e279(&(DialogWin &)*o);
    } else {
        x -= o->rect.left;
        y -= o->rect.top;
        MapWin_WinPos2MapPos(o, x, y, &mx, &my);
        if (g_game.view_mode == 0 && g_units[g_game.selected_unit].x == mx && g_units[g_game.selected_unit].y == my)
            track_popup_submenu_1038_e2b5(&(DialogWin &)*o, &g_menu_owner, 2, x, y);
    }
    MSDoAllTasks_1038_e2a2();
    g_map_click_busy = 0;
}
