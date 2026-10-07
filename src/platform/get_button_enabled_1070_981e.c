/* get_button_enabled_1070_981e: candidate for 1070:981e (25 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_button_enabled_1070_981e
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x1a of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_w_1a_1070_981e: a button's enabled flag. At 0 bitmap_button_proc
 * ignores the press and paints it grey, MSPushButton returns early, and focus_walk_siblings and
 * dialog_mnemonic skip kind-6 controls that have it clear.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

int get_button_enabled_1070_981e(SButton *button)
{
    return button->enabled;
}
