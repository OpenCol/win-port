/* orders_wait: candidate for 1008:0051 (30 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The WAIT order: `GameBrains_EndUnitTurn(&g_game_brains, 1)`. `GameBrains_KeyboardEvent` calls it for
 * the 'W' key and for nothing else, after clearing the record's +4 as every order there does.
 *
 * The 1 is GameBrains_EndUnitTurn's force flag. Without it, a unit that is still selectable keeps
 * the turn; with it, the turn moves on regardless -- which is what waiting is: leave this
 * unit where it stands and go to the next one that wants orders.
 *
 * The call has a NOP before its `push cs`, the five-byte form TLINK leaves for an `extern`,
 * so GameBrains_EndUnitTurn is declared here and not defined (rule 47).
 *
 * RENAMED 2026-09-26 from order_wait: the Mac build's orders_wait(void), bound by call-sequence alignment: in GameBrains_KeyboardEvent the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (0), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "records.h"
extern GameBrains g_game_brains;           /* SEG20:0x40d6 */
extern int GameBrains_EndUnitTurn(void *brains, int force);

void orders_wait(void)
{
    GameBrains_EndUnitTurn(&g_game_brains, 1);
}
