/* min_char_ptr_1048_efbd: candidate for 1048:efbd (44 bytes).  Verdict: EXACT.
 *
 * Of two char pointers, the one pointing at the smaller value (the second on a tie); segment 1048's copy.
 */

#include "colonization.h"

char * min_char_ptr_1048_efbd(char *a, char *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
