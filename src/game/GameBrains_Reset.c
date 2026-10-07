/* GameBrains_Reset: candidate for 1008:378f (41 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Writes three constants into the turn context: 100 at +4, then 0 at +6 and at +2. Each store
 * reloads the pointer, which is what three separate statements compile to.
 *
 * `load_game` is the caller, with `&g_game_brains` -- the object `GameBrains_EndUnitTurn` and
 * `GameBrains_KeyboardEvent` also take. The name says what the function does rather than what the
 * three words mean, because nothing here establishes that; `get_w_6` reads the one at
 * +6 and is named the same way.
 *
 * RENAMED 2026-09-26 from set_defaults: the Mac build's GameBrains::Reset(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between GameBrains_MoveUnit and GameBrains_KeyboardEvent, where the Mac gap holds 1 functions and the Windows one 2, so the pairing inside it is by size and arity (the Mac body is 1.22x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order. next-session had left GameBrains Init/Reset open between set_defaults and GameBrains_Init; layout puts Reset here, between GameBrains_MoveUnit and GameBrains_KeyboardEvent.
 */

#include "colonization.h"

#include "records.h"

void GameBrains_Reset(GameBrains *brains)
{
    brains->phase = 100;
    brains->awaiting_human = 0;
    brains->step = 0;
}
