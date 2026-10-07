/* rect_width: candidate for 1070:dc05 (31 bytes).  Verdict: EXACT.
 * size-source: 31, not the census 27. The trailing 4 bytes are the dead second epilogue.
 *
 * `r->right - r->left`, this segment's copy. It sits at the end of a run of three same-sized
 * functions at 1070:dbc7, dbe6 and dc05 whose first two are one-line forwarders, which is
 * what put it in reach: the run was found by arithmetic, not by reading.
 *
 * Both halves are loaded through their own `LES BX`, because `-Od` keeps nothing.
 */

#include "colonization.h"

#include "w16types.h"
int rect_width(RECT *r)
{
    return r->right - r->left;
}
