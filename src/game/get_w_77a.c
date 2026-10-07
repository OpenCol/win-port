/* get_w_77a: candidate for 1050:8cbe (26 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 26, not the census 22. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x77a of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int get_w_77a(ReportWin *r)
{
    return r->kind;
}
