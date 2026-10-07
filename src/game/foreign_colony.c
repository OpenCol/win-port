/* foreign_colony: candidate for 1028:353c (25 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * This module's one-line forwarder to `colony_plan`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 *
 * RENAMED 2026-09-26 from ai_manage_colony_1028_353c: the forwarder is named for the routine it reaches, and that routine is now colony_plan (the Mac build's name for what was ai_manage_colony); the address suffix stays because colony_plan is taken.
 *
 * RENAMED 2026-09-27 from colony_plan_1028_353c: Mac foreign_colony(short) is 40 bytes. Its only caller is foreign_turn and its only callee is colony_plan. 1028:353c is a 25-byte forwarder whose only caller is foreign_turn (1028:4c8b) and whose only callee is colony_plan (1018:04e0). The call graph closes in both directions: Mac colony_plan's only caller is foreign_colony, and Windows colony_plan's only caller is 353c. Arity: (short) against colony_plan_1028_353c(int colony). The current name is descriptive ('named for the routine it reaches', with an address suffix because colony_plan is taken), so a developers' name replaces a placeholder. Apply this only after 1020:9126 has moved to foreign_cargo.
 */

#include "colonization.h"

extern void colony_plan(int colony);

void foreign_colony(int colony)
{
    colony_plan(colony);
}
