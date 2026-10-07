/* GameBrains_CursorUpdate: candidate for 1008:3038 (269 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Brings the map up to date with the selected unit (SEG20:0x7794). In the unit view (SEG20:0x7792
 * zero) the cursor is moved onto the unit, and if the unit is inside the visible map it is
 * redrawn through `GenerateActiveUnit` -- flashing only while DGROUP:0x76 is clear.
 * Otherwise the square under the cursor is repainted. In the other view, the cursor itself is
 * drawn on the map window, but only while DGROUP:0x76 is set.
 *
 * `!g_cycle_flag` as an argument is the branch-free `NEG AX / SBB AX,AX / INC AX` Borland emits
 * for a logical not used as a value. DGROUP:0x76 is read as a NEAR global in this module, where
 * `set_center` declares it (§114).
 *
 * RENAMED 2026-09-26 from refresh_unit_cursor: the Mac build's GameBrains::CursorUpdate(void): its one parameter is the unread `this`; it brings the map and cursor up to date with the selected unit. Every function of the GameBrains module (1008:2927..3cc8) is called on the one object g_game_brains (SEG20:0x40d6), and the width matches.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

#include "classes.h"

extern int on_screen(int x, int y);
extern void GenerateActiveUnit(int flag);
extern void GenerateUnitsRegion(int x, int y, int w, int h);
extern void MapWin_DrawCursor(void *w, int x, int y);

void GameBrains_CursorUpdate(GameBrains *unused1)
{
    if (g_game.view_mode == 0)
        set_cursor_pos(g_units[g_game.selected_unit].x, g_units[g_game.selected_unit].y);
    if (g_game.view_mode == 0
        && on_screen(g_units[g_game.selected_unit].x, g_units[g_game.selected_unit].y) != 0)
        GenerateActiveUnit(!g_cycle_flag);
    else
        GenerateUnitsRegion(g_cursor_x, g_cursor_y, 1, 1);
    if (g_game.view_mode != 0 && g_cycle_flag != 0)
        MapWin_DrawCursor(&g_map_window, g_cursor_x, g_cursor_y);
}
