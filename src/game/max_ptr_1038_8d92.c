/* max_ptr_1038_8d92: candidate for 1038:8d92 (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the larger value (the second on
 * a tie).
 */

#include "colonization.h"

int * max_ptr_1038_8d92(int *a, int *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
