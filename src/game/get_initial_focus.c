/* get_initial_focus: candidate for 1060:ac00 (25 bytes).  Verdict: EXACT.
 *
 * The byte at +0x16 of the control record, returned without promotion.
 *
 * RENAMED 2026-09-26 from byte_at_16: the control's initial-focus mark. find_control returns the
 * first control with it set, and DialogWin_SetDefaultControlFocus gives that control the focus, falling back to the
 * first control when none is marked.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

char get_initial_focus(DlgControl *ctl)
{
    return ctl->initial_focus;
}
