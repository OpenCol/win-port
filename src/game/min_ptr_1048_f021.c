/* min_ptr_1048_f021: candidate for 1048:f021 (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the smaller value (the second on a tie); segment 1048's copy.
 */

#include "colonization.h"

int * min_ptr_1048_f021(int *a, int *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
