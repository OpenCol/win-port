/* get_items_1070_99c5: candidate for 1070:99c5 (29 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_items_1070_99c5
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * The pointer at +0x2e of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

#include "records.h"
RadioItem *get_items_1070_99c5(SRadioSet *radios)
{
    return (RadioItem *)radios->items;
}
