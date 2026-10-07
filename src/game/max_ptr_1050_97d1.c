/* max_ptr_1050_97d1: candidate for 1050:97d1 (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the larger value (the second on a
 * tie); segment 1050's copy.
 */

#include "colonization.h"

int * max_ptr_1050_97d1(int *a, int *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
