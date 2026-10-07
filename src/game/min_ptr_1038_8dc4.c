/* min_ptr_1038_8dc4: candidate for 1038:8dc4 (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the smaller value (the second on a tie); segment 1038's copy.
 */

#include "colonization.h"

int * min_ptr_1038_8dc4(int *a, int *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
