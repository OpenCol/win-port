/* GameBrains_UnitActivate: candidate for 1008:3ca7 (24 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets DGROUP:0x4b00 and hands the square to `orders_activate`. Both callers are
 * `MapWin_LeftMouseUp`, with the cursor's map square.
 *
 * THE FIRST ARGUMENT `brains` IS NEVER READ. Both callers pass `&g_game_brains`, the turn context that
 * `GameBrains_EndUnitTurn` and `GameBrains_KeyboardEvent` also take, so the parameter is the caller's claim
 * rather than this function's -- the body uses only the two coordinates (RE_STATUS 119).
 *
 * `g_player_took_control` IS NAMED NOW, and this header said until 2026-09-24 that nothing
 * established what it meant. The three sites together do: `GameBrains_DoHuman` clears it at the
 * top of a nation's turn (phase 100) and raises it again once the map is up and waiting for
 * input (phase 1, map view); this function raises it when the player clicks a unit. So it is a
 * latch for "the player has been handed the turn". `GameBrains_EndUnitTurn` spends it -- when no unit
 * is left to select AND the latch is up AND the end-of-turn flag is clear, it drops
 * `g_game.human_turn_active` and the turn ends by itself. Without the latch an AI-driven turn with no
 * selectable units would end the same way, which is what the clear at phase 100 prevents.
 *
 * RENAMED 2026-09-26 from pick_unit_at: the Mac build's GameBrains::UnitActivate(short, short): this plus a map square, activating the unit picked there (MapWin_LeftMouseUp's calls). Every function of the GameBrains module (1008:2927..3cc8) is called on the one object g_game_brains (SEG20:0x40d6), and the width matches.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int g_stub_a;
extern int orders_activate(int x, int y);

void GameBrains_UnitActivate(GameBrains *brains, int x, int y)
{
    g_player_took_control = 1;
    orders_activate(x, y);
}
