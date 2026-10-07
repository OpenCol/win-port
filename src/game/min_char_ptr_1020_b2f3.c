/* min_char_ptr_1020_b2f3: candidate for 1020:b2f3 (44 bytes).  Verdict: EXACT.
 *
 * Of two char pointers, the one pointing at the smaller value (the second on a tie); segment 1020's copy.
 */

#include "colonization.h"

char * min_char_ptr_1020_b2f3(char *a, char *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
