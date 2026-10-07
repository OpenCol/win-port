/* max_ptr_1040_b582: candidate for 1040:b582 (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the larger value (the second on
 * a tie); segment 1040's copy.
 */

#include "colonization.h"

int * max_ptr_1040_b582(int *a, int *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
