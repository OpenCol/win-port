/* GameBrains_UnitGotoXY: candidate for 1008:3595 (91 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Puts the unit at SEG20:0x7794 on mission 3 with the two coordinates the caller
 * gives, clears the word at +4 of the caller's record and returns 0.
 *
 * RENAMED 2026-09-26 from arrived_unit_mission: the Mac build's GameBrains::UnitGotoXY(short, short): this plus two coordinates, putting the selected unit on mission 3 (go to) with them. Every function of the GameBrains module (1008:2927..3cc8) is called on the one object g_game_brains (SEG20:0x40d6), and the width matches.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

int GameBrains_UnitGotoXY(GameBrains *r, int dest_x, int dest_y)
{
    register int u = g_game.selected_unit;

    g_units[u].orders = 3;
    g_units[u].dest_x = dest_x;
    g_units[u].dest_y = dest_y;
    r->phase = 0;
    return 0;
}
