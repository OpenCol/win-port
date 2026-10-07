/* get_checked_1050_8d83: candidate for 1050:8d83 (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x1e of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_w_1e_1050_8d83: a checkbox's checked state. checkbox_click and
 * checkbox_wndproc toggle it 1/0 on a click and pass it to the change handler, paint picks the on or
 * off bitmap from it, and Popup_exec reads each popup check back through it.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int get_checked_1050_8d83(SCheckBox *check)
{
    return check->checked;
}
