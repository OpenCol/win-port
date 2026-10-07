/* PORT: this file's own declarations of ColChild_ActivateMessage, ColChild_DrawAnyMessage removed: the header declares them from the definition (W1 step 3). */
/* colony_error: candidate for 1010:c0b0 (53 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: colony_error
 *
 * Cancels the colony window's current message, appends text `idx` and activates it as kind
 * 3. The two same-module helpers are defined here because the shipped calls to them are the
 * compiler's own `push cs / call `.
 *
 * RENAMED 2026-09-26 from colony_cancel_then_message: the Mac build's colony_error(short, long), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between colony_message and try_to_set_job, where the Mac gap holds 2 functions and the Windows one 3, so the pairing inside it is by size and arity (the Mac body is 1.21x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own ColonyWin removed: include/gen/classes.h define it once (W1 step 2). */

#include "classes.h"
extern void ColChild_CancelMessage(ColChild *child, int a);
extern void ColChild_AddToMessage(ColChild *child, int idx);

void colony_error(int idx, long ms)
{
    ColChild_CancelMessage((ColChild *)&g_colony_window, 1);
    colony_message(idx);
    colony_activate_message(3, ms);
}
