/* get_check_enabled_1070_997b: candidate for 1070:997b (25 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_check_enabled_1070_997b
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x20 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_w_20_1070_997b: a checkbox's enabled flag. At 0 checkbox_wndproc
 * paints the grey bitmap and the disabled text colour and drops focus, and focus_walk_siblings and
 * dialog_mnemonic skip kind-2 controls that have it clear.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

int get_check_enabled_1070_997b(SCheckBox *check)
{
    return check->enabled;
}
