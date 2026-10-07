/* min_ptr_1010_f2b7: candidate for 1010:f2b7 (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the smaller value (the second on a tie); segment 1010's copy.
 */

#include "colonization.h"

int * min_ptr_1010_f2b7(int *a, int *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
