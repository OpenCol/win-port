/* get_w_a: candidate for 1008:d0fe (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0xa of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

int get_w_a(GamePrefs *r)
{
    return r->use_default_layout;
}
