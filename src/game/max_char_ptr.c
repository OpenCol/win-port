/* max_char_ptr: candidate for 1030:f4d8 (44 bytes).  Verdict: EXACT.
 *
 * Of two char pointers, the one pointing at the larger value (the second on a tie); segment 1030's copy.
 */

#include "colonization.h"

char * max_char_ptr(char *a, char *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
