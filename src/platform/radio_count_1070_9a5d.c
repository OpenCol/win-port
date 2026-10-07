/* radio_count_1070_9a5d: candidate for 1070:9a5d (25 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: radio_count_1070_9a5d
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x26 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_w_26_1070_9a5d: the number of buttons in a radio set. radio_wndproc bounds
 * and wraps the arrow-key index with it, and dialog_mnemonic walks that many items of a kind-3 control.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

int radio_count_1070_9a5d(SRadioSet *radios)
{
    return radios->count;
}
