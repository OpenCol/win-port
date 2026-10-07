/* GameBrains_NextUnitTurn: candidate for 1008:3cc8 (23 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Zeroes the word at +4 of the record it is given, and does nothing else.
 *
 * Both call sites (SEG10:0xc541 and 0xc556) pass `g_game_brains` at SEG20:0x40d6 and sit in
 * the same key-command switch, each immediately after an order that finishes a unit's turn --
 * `orders_route` at one and `orders_europe` at the other. A third arm of that switch
 * calls `GameBrains_KeyboardEvent(&g_game_brains, 0x47)` instead and does NOT clear the field.
 *
 * THE FIELD IS NAMED FROM THE RECORD'S OTHER USERS. Every function that takes this record takes it as `void *`
 * -- `GameBrains_SetViewMode`, `GameBrains_MoveUnit`, `GameBrains_UnitGotoXY`, `GameBrains_UnitActivate` all do -- but
 * `GameBrains_Reset` and `GameBrains_DoHuman` declare it, and there +4 is the turn phase:
 * Reset starts it at 100 and DoHuman switches on it and sets it back to 0.
 *
 * RENAMED 2026-09-26 from clear_game_record_w4: the word at +4 of g_game_brains is the turn phase (GameBrains_DoHuman steps it; GameBrains_Reset starts it at 100); this clears it.
 *
 * RENAMED 2026-09-27 from clear_turn_phase: the Mac build's GameBrains_NextUnitTurn, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Zeroes g_game_brains' turn phase (+4); its only caller is MainWin_MainMenuProc, right after orders_route and return_to_europe finish a unit's turn, and Mac NextUnitTurn is a call-free leaf whose only caller is MainMenuProc; this + void.
 */

#include "colonization.h"

#include "records.h"

void GameBrains_NextUnitTurn(GameBrains *r)
{
    r->phase = 0;
}
