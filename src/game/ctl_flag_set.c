/* ctl_flag_set: candidate for 1058:c1dd (38 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Sets the word at +0x1a of a control record and invalidates its window, so the change is
 * repainted. Its twin at 1058:c199 (`ctl_flag_clear`, also 38 bytes) clears the same word to 0 and is otherwise
 * byte-identical -- one pair, one flag, two values.
 *
 * The handle is at +0x10, which is where every control record in this corpus keeps it.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void ctl_flag_set(SButton *ctl)
{
    ctl->enabled = 1;
    invalidate_hwnd(ctl->hwnd);
}
