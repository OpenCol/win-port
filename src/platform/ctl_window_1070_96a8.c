/* ctl_window_1070_96a8: candidate for 1070:96a8 (29 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ctl_window_1070_96a8
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * The owner WINDOW of a dialog control: the pointer at +0x4 of the control record.
 * A one-line accessor, 20-odd bytes of prologue and epilogue around a single move.
 *
 * `DlgControl`'s +0x4 field is `win`, and the 86 call sites agree: every one passes a
 * control and hands the result to `SetEventWindow`, to the window's own port
 * accessor, or -- in `link_derived_to_ctl` -- writes the control back into the window at
 * +0x90. One of three per-module copies (1050:948e, 1060:ab51, 1070:96a8).
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void *ctl_window_1070_96a8(DlgControl *ctl)
{
    return ctl->win;
}
