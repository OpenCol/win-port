/* MapWin_LeftMouseClick: candidate for 1038:9371 (411 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_LeftMouseClick
 * name-source: sibling-build
 *
 * A left click on the map window, re-entry flag at DGROUP:0x1c50 -- the third of the four
 * 0x1c5x flags, beside `MapWin_LeftMouseUp`'s 0x1c57 and `MapWin_RightMouseClick`'s 0x1c52.
 * Outside the map rectangle the window is closed; inside, the drag state at +0x784 decides.
 *
 * -1, meaning no drag, is the plain click: the colony under the pointer opens when it is the
 * player's or god mode is on, and otherwise the map recentres on the square. In view mode the
 * cursor follows and the view record is laid out again. A drag of 0 does NOTHING, and any
 * other value is a direction handed to `GameBrains_MoveUnit` as `drag - 1` -- the same
 * three cases `MapWin_LeftMouseUp` has, with the unit-activation arm left out.
 *
 * `else if (o->drag == 0) { }` IS AN EMPTY BRANCH IN THE SHIPPED BYTES: the test jumps INTO
 * the last arm and then over it, which is rule 54's empty-`else` shape. Dropping the empty
 * branch and writing `else if (o->drag != 0)` gives one jump past the body instead of two.
 *
 * The nation comparison is done in BYTES -- `CMP AL,ES:[0x7798]`, no widening either side --
 * so both `Colony+0x1a` and the word at SEG20:0x7798 are read through `char` declarations here,
 * as they are in `MapWin_LeftMouseUp`.
 *
 * `win_show_and_focus_1038_e279` and `MSDoAllTasks_1038_e2a2` come through the four-byte `PUSH CS`
 * form and are stubs here (rule 47). `MapWin_RightMouseClick` declares the same two `extern`
 * over the same four-byte sites and still grades RELOCATABLE, because the comparator will
 * collapse a five-byte call to the four-byte form -- but it then has to forgive every jump
 * displacement after it as "one out, same destination". The stub form reproduces them exactly,
 * which is the reason to prefer it.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, MapWin, ColonyWin, StatWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-








#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */



extern GameBrains g_game_brains;            /* SEG20:0x40d6 */

extern StatWin g_stat_win;           /* SEG20:0x1566 */
extern void *GetEventWindow(void);
extern int MapWin_WinPos2MapPos(MapWin *o, int x, int y, int *mx, int *my);

extern void ColonyWin_Show(void *w, int colony);
extern void MapWin_Reposition(MapWin *o, int x, int y);

extern void GameWin_Redraw(GameWin *rec);
extern void GameBrains_MoveUnit(void *rec, int dir);

/* same object: 4-byte `push cs; call ` */
extern "C" void win_show_and_focus_1038_e279(void *w);

void MapWin_LeftMouseClick(int x, int y)
{
    MapWin *o;
    int mx;
    int my;
    int colony;
    EventWin *w;

    if (g_map_lclick_busy != 0)
        return;
    g_map_lclick_busy = 1;
    w = (EventWin *)GetEventWindow();
    o = (MapWin *)w;
    if (o->rect.left > x || o->rect.right < x || o->rect.top > y || o->rect.bottom < y) {
        win_show_and_focus_1038_e279(&(DialogWin &)*o);
    } else {
        x -= o->rect.left;
        y -= o->rect.top;
        if (o->drag == -1) {
            MapWin_WinPos2MapPos(o, x, y, &mx, &my);
            colony = find_colony(mx, my);
            if (colony >= 0 && ((char)g_colonies[colony].nation == ((char)g_game.view_nation) || g_game.reveal_all != 0))
                ColonyWin_Show(&g_colony_window, colony);
            else
                MapWin_Reposition(o, x, y);
            if (g_game.view_mode == 1) {
                set_cursor_pos(mx, my);
                GameWin_Redraw((GameWin *)&g_stat_win);
            }
        } else if (o->drag == 0) {
        } else {
            if (g_game.view_mode != 1)
                GameBrains_MoveUnit(&g_game_brains, o->drag - 1);
        }
    }
    MSDoAllTasks_1038_e2a2();
    g_map_lclick_busy = 0;
}
