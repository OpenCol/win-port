/* PORT: this file's own declarations of ReportWin_DoReport_parent removed: the header declares it from the definition (W1 step 3). */
/* ReportWin_DoReport: candidate for 1058:021c (41 bytes).  Verdict: RELOCATABLE.
 *
 * ReportWin_DoReport_parent(o, kind, flag, NULL).
 *
 * THE SECOND ARGUMENT IS NOT A POINTER, though this file said `void *b` until
 * 2026-09-24. `ReportWin_DoReport_parent` reads `(ReportWin *o, int kind,
 * char flag, void *rec)`, and both callers pass a kind and a flag as two words.
 * Four bytes either way, so the byte match had nothing to say; the callee one step
 * further down did.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int ReportWin_DoReport(ReportWin *o, int kind, char flag)
{
    return ReportWin_DoReport_parent(o, kind, flag, 0);
}
