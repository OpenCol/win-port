/* min_char_ptr_1030_f50a: candidate for 1030:f50a (44 bytes).  Verdict: EXACT.
 *
 * Of two char pointers, the one pointing at the smaller value (the second on a tie); segment 1030's copy.
 */

#include "colonization.h"

char * min_char_ptr_1030_f50a(char *a, char *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
