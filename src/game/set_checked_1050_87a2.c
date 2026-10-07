/* set_checked_1050_87a2: candidate for 1050:87a2 (24 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * The setter paired with `get_checked_1070_9921`: stores a word at +0x1e of a control record.
 * `checkbox_click` is what reads that word and writes it back inverted, which is
 * what makes +0x1e the checked state.
 *
 * TRANSFERRED: byte-identical to `set_checked_1070_993a` (1070:993a), one of 2 copies of this
 * function in the image. Same source, this segment's copy.
 *
 * RENAMED 2026-09-26 from set_w_1e_1050_87a2: stores a checkbox's checked state. checkbox_click
 * and checkbox_wndproc store 1 or 0 to toggle it, and Popup_make_controls seeds each popup check from
 * its entry's flags & 4.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_checked_1050_87a2(SCheckBox *check, int checked)
{
    check->checked = checked;
}
