/* ctable_seed: candidate for 1068:98ca (26 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ctable_seed
 * name-source: descriptive
 * size-source: 26, not the census 22. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x406 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_w_406: the CTable's seed. CTable_ctor and CTable_UpdatePal set it
 * to a fresh non-zero random value whenever the colours change, and GRPort_ColorSync compares it
 * to the port's recorded id to skip re-selecting an unchanged table -- the Mac ctSeed.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int ctable_seed(CTable *pal)
{
    return pal->seed;
}
