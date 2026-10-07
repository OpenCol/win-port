/* ctl_flag_clear: candidate for 1058:c199 (38 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Clears the word at +0x1a of a control record and invalidates its window, so the change is
 * repainted. Its twin at 1058:c1dd, the same 38 bytes, sets the same word to 1 and is otherwise
 * byte-identical -- one pair, one flag, two values.
 *
 * The handle is at +0x10, which is where every control record in this corpus keeps it.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void ctl_flag_clear(SButton *control)
{
    control->enabled = 0;
    invalidate_hwnd(control->hwnd);
}
