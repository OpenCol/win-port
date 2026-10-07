/* Popup_set_width: candidate for 1050:0cee (25 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Stores its second argument in the popup's word at +0x358 and does nothing else.
 *
 * WHAT THE CALLERS SAY IT IS. Four of them: `(&popup, 0x1e0)`, `(l.popup, 300)`, `(p, 0x1b8)`
 * and `(p, rect_width_1050_830c(r))` -- 480, 300, 440, and a rectangle's width. A width is the
 * only thing all four are, and the last one names itself.
 *
 * IT NEEDED A RECONSTRUCTION RATHER THAN A RENAME. `Popup_start` reaches it through
 * the four-byte `PUSH CS / CALL NEAR`, so `check_call_forms` resolves the callee through the
 * symbol map -- and the map takes a name from a function's own reconstruction, which this had
 * none of. Renaming the stub alone was tried and reverted; this is the repair that sticks.
 *
 * RENAMED 2026-09-26 from popup_set_width: the Mac build's Popup::set_width(int): this plus the width. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

void Popup_set_width(Popup *popup, int width)
{
    popup->width = width;
}
