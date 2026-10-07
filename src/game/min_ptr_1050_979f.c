/* min_ptr_1050_979f: candidate for 1050:979f (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the smaller value (the second on a
 * tie); segment 1050's copy.
 */

#include "colonization.h"

int * min_ptr_1050_979f(int *a, int *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
