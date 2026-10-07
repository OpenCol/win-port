/* get_cur_item_1050_848f: candidate for 1050:848f (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x2a of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int get_cur_item_1050_848f(SRadioSet *radios)
{
    return radios->cur_item;
}
