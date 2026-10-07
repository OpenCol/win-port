/* max_ptr_1048_efef: candidate for 1048:efef (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the larger value (the second on a tie); segment 1048's copy.
 */

#include "colonization.h"

int * max_ptr_1048_efef(int *a, int *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
