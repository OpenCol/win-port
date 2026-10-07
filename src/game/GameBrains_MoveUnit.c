/* GameBrains_MoveUnit: candidate for 1008:35f0 (198 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The other half of `GameBrains_UnitGotoXY`'s pair, and the same caller's record:
 * what happens on screen when a unit arrives. A non-negative direction announces the
 * arrival -- `execute_move_unit` with the direction's two offsets from the tables at
 * SEG27:0x0 and SEG27:0x9 -- and scrolls the map to the cursor; in any view but the map
 * (SEG20:0x7792) `move_cursor` gets the direction instead.
 *
 * Then the selection is re-checked: a record whose +4 is 1 goes back to 0 when the selected
 * unit can no longer be selected, which is the deselect that a unit becoming unavailable
 * during its own arrival needs. Both windows are relaid out last.
 *
 * The direction is a `register` PARAMETER: the shipped prologue loads SI from [BP+0xa]
 * before anything else, which is the parameter-copy form of rule 40 rather than a local
 * assigned from a parameter.
 *
 * The two direction tables are `signed char` and reach an `int` argument, which is what the
 * `CBW` after each load says; an `unsigned char` would be `MOV AH,0`.
 *
 * RENAMED 2026-09-26 from arrived_landfall: the Mac build's GameBrains::MoveUnit(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller KeyboardEvent; caller LeftMouseUp; callee Redraw; callee unit_is_active), the runner-up gets 2, and the arity agrees (2). Beyond the anchors, every Mac callee pairs with one of this function's: execute_move_unit/execute_move_unit, possibly_center/possibly_center, MainWin::CheckMenuItems/MainWin_CheckMenuItems, move_cursor/an empty stub, GameWin::Redraw, unit_is_active.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

#include "classes.h"
extern MainWin g_main_win;             /* SEG20:0x5ca */
typedef struct { char pad[1]; } StatWin;
extern StatWin g_stat_win;           /* SEG20:0x1566 */
extern int execute_move_unit(int dx, int dy);
extern int possibly_center(int x1, int y1, int x2, int y2, int mode);
extern void move_cursor(int dir);
extern int unit_is_active(int u);
extern void MainWin_CheckMenuItems(void *w);
extern void GameWin_Redraw(GameWin *w);

void GameBrains_MoveUnit(GameBrains *r, register int dir)
{
    if (dir >= 0) {
        if (g_game.view_mode == 0) {
            execute_move_unit(g_dir_dx[dir], g_dir_dy[dir]);
            possibly_center(g_cursor_x, g_cursor_y, g_cursor_x, g_cursor_y, 0);
        } else {
            move_cursor(dir);
        }
    }
    if (r->phase == 1 && unit_is_active(g_game.selected_unit) == 0)
        r->phase = 0;
    MainWin_CheckMenuItems(&g_main_win);
    GameWin_Redraw((GameWin *)&g_stat_win);
}
