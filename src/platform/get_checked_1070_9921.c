/* get_checked_1070_9921: candidate for 1070:9921 (25 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_checked_1070_9921
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x1e of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_w_1e_1070_9921: a checkbox's checked state. checkbox_click and
 * checkbox_wndproc toggle it 1/0 on a click and pass it to the change handler, paint picks the on or
 * off bitmap from it, and Popup_exec reads each popup check back through the game module's copy, get_checked_1050_8d83.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

int get_checked_1070_9921(SCheckBox *ctl)
{
    return ctl->checked;
}
