/* get_check_enabled_1060_ac63: candidate for 1060:ac63 (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x20 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_w_20_1060_ac63: a checkbox's enabled flag. At 0 checkbox_wndproc
 * paints the grey bitmap and the disabled text colour and drops focus, and focus_walk_siblings and
 * dialog_mnemonic skip kind-2 controls that have it clear.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int get_check_enabled_1060_ac63(SCheckBox *check)
{
    return check->enabled;
}
