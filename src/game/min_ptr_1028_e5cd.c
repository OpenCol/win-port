/* min_ptr_1028_e5cd: candidate for 1028:e5cd (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the smaller value (the second on a tie); segment 1028's copy.
 */

#include "colonization.h"

int * min_ptr_1028_e5cd(int *a, int *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
