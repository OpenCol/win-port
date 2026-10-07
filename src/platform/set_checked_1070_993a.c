/* set_checked_1070_993a: candidate for 1070:993a (24 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: set_checked_1070_993a
 * name-source: descriptive
 *
 * The setter paired with `get_checked_1070_9921`: stores a word at +0x1e of a control record.
 * `checkbox_click` is what reads that word and writes it back inverted, which is
 * what makes +0x1e the checked state.
 *
 * RENAMED 2026-09-26 from set_w_1e_1070_993a: stores a checkbox's checked state. checkbox_click
 * and checkbox_wndproc store 1 or 0 to toggle it, and Popup_make_controls seeds each popup check from
 * its entry's flags & 4.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void set_checked_1070_993a(SCheckBox *check, int checked)
{
    check->checked = checked;
}
