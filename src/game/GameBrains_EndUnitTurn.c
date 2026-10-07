/* PORT: this file's own declarations of MainWin_CheckMenuItems removed: the header declares it from the definition (W1 step 3). */
/* GameBrains_EndUnitTurn: candidate for 1008:2dbc (636 bytes).  Verdict: RELOCATABLE.
 * symbol: GameBrains_EndUnitTurn
 * name-source: sibling-build
 * size-source: 636, not the census 620. The trailing bytes are the dead second epilogue and
 * the switch's five-entry jump table, which the census cuts off at the first RETF.
 *
 * Moves the turn on to the next unit that wants orders. The unit standing now has its square
 * explored and its tile redrawn (with DGROUP:0x1ccc down across the redraw and back up after);
 * then, unless it is still selectable and the caller did not force the move,
 * `get_next_active_unit` picks the next one and `unit_activate` brings the
 * display to it.
 *
 * WHAT THE NEW UNIT'S ORDERS SAY DECIDES BOTH OUTPUTS. Orders 5, 6, 8 and 9 mean the unit is
 * doing something and needs no attention: the function returns 1 and the map is not moved.
 * Anything else -- 7 included, which the jump table sends to the default -- returns 0 and asks
 * for the map to be scrolled to it.
 *
 * With a unit selected the view mode and the pending-message flag come down, the cursor goes to
 * the unit (scrolling only when the switch asked for it), and the orders panel and the view
 * record are laid out again. With none, the pending-message flag goes UP instead, the pieces
 * view is drawn, and the two menu items are greyed.
 *
 * `held` is the return value and `scroll` the flag: two register locals, SI then DI in
 * declaration order, so `scroll` is SI and `held` DI (rule 53).
 *
 * The `case 7` hole in the table is what makes this a `switch` and not an `if` chain: Borland
 * builds a dense table from 5 to 9 and fills the hole with the default's address.
 *
 * RENAMED 2026-09-26 from select_next_unit: the Mac build's GameBrains::EndUnitTurn(short): this plus a force flag, ending the current unit's turn and moving on to the next that wants orders. Every function of the GameBrains module (1008:2927..3cc8) is called on the one object g_game_brains (SEG20:0x40d6), and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "gameflag.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
#include "classes.h"
extern MainWin g_main_win;             /* SEG20:0x5ca */
typedef struct { char pad[1]; } StatWin;
extern StatWin g_stat_win;         /* SEG20:0x1566 */

extern void DrawMapRegion(int x, int y, int a, int b, int flag);
extern int unit_special_active(int u);
extern int get_next_active_unit(int u);
extern void unit_activate(int u);

extern int possibly_center(int x1, int y1, int x2, int y2, int flag);
extern void GameWin_Redraw(GameWin *w);

/* same object: 4-byte `push cs; call ` */
extern void GameBrains_SetViewMode(void *rec);
extern void FreezeMenus(void);
extern void check_tut(void);

int GameBrains_EndUnitTurn(GameBrains *brains, int force)
{
    register int scroll;
    register int held;

    held = 0;
    scroll = g_game.view_mode != 0;
    if (g_game.selected_unit >= 0) {
        g_unit_blinking = 0;
        explore_square(g_game.selected_unit);
        DrawMapRegion(g_units[g_game.selected_unit].x, g_units[g_game.selected_unit].y,
                                  1, 1, 1);
        g_unit_blinking = 1;
    }
    if (unit_special_active(g_game.selected_unit) == 0 || force != 0) {
        unit_activate(get_next_active_unit(g_game.selected_unit));
        switch (g_units[g_game.selected_unit].orders) {
        case 5:
        case 6:
        case 8:
        case 9:
            scroll = 0;
            held = 1;
            break;
        default:
            scroll = 1;
            break;
        }
    }
    if (g_game.selected_unit >= 0) {
        g_game.view_mode = 0;
        g_game.end_of_turn_pending = 0;
        if (held == 0) {
            if (on_map(g_units[g_game.selected_unit].x, g_units[g_game.selected_unit].y) != 0) {
                set_cursor_pos(g_units[g_game.selected_unit].x,
                                         g_units[g_game.selected_unit].y);
                if (scroll != 0)
                    possibly_center(g_cursor_x, g_cursor_y,
                                                 g_cursor_x, g_cursor_y, 0);
            }
        }
        MainWin_CheckMenuItems(&g_main_win);
        GameWin_Redraw((GameWin *)&g_stat_win);
    } else {
        g_game.end_of_turn_pending = 1;
        GameBrains_SetViewMode(brains);
        if (g_player_took_control != 0 && (HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_END_OF_TURN) == 0)
            g_game.human_turn_active = 0;
        FreezeMenus();
        scroll = 0;
    }
    if (g_game.selected_unit >= 0 && (g_game.game_flags & GAMEFLAG_TUTORIAL))
        check_tut();
    return held;
}
