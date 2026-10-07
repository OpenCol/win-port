/* popup_get_first_edit_text: candidate for 1050:1009 (35 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `popup_get_edit_text(c, 0, p)`: applies `p` to item 0 of a control. It sits directly after
 * popup_get_edit_text itself.
 *
 * The call is the four-byte `push cs; call ` with no NOP, so the callee is defined here
 * as a stub (rule 47).
 *
 * RENAMED 2026-09-26 from ctl_item_apply_first: the forwarder that calls popup_get_edit_text (was ctl_item_apply) with index 0: it reads the first edit box's text out, the direction the old name had backwards.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void popup_get_edit_text(Popup *control, int i, void *p);

void popup_get_first_edit_text(Popup *ctl, void *p)
{
    popup_get_edit_text(ctl, 0, p);
}
