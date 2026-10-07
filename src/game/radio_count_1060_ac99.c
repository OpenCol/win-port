/* radio_count_1060_ac99: candidate for 1060:ac99 (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x26 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_w_26_1060_ac99: the number of buttons in a radio set. dialog_mnemonic walks
 * that many items of a kind-3 control looking for the mnemonic; radio_wndproc's copy bounds the index.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int radio_count_1060_ac99(SRadioSet *radios)
{
    return radios->count;
}
