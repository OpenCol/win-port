/* max_ptr_1050_97d1: candidate for 1050:97d1 (50 bytes).  Verdict: EXACT.
 *
 * Of two int pointers, the one pointing at the larger value (the second on a
 * tie); segment 1050's copy.
 */

#include "colonization.h"

/* PORT: i16, Borland's int: every caller hands it two Popup members or locals beside them (W1). */
i16 *max_ptr_1050_97d1(i16 *a, i16 *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
