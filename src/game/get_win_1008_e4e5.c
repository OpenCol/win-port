/* get_win_1008_e4e5: candidate for 1008:e4e5 (29 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * The pointer at +0x6 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 */

#include "colonization.h"

#include "classes.h"

void *get_win_1008_e4e5(Screen *r)
{
    return r->win;
}
