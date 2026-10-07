/* get_mci: candidate for 1070:a04b (31 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 31, not the census 27. The trailing 4 bytes are the dead second epilogue.
 *
 * The pointer at +0x8c of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 */

#include "colonization.h"

#include "records.h"

Mci *get_mci(Audio *r)
{
    return (Mci *)r->mci;
}
