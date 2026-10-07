/* get_w_6: candidate for 1048:e56c (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x6 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 */

#include "colonization.h"

#include "records.h"

int get_w_6(GameBrains *r)
{
    return r->awaiting_human;
}
