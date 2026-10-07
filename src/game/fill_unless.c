/* fill_unless: candidate for 1040:6075 (46 bytes).  Verdict: EXACT.
 *
 * Walks from p while the pointer is non-null, replacing every byte other
 * than `keep` with `fill`.
 */

#include "colonization.h"

void fill_unless(char *p, char keep, char fill)
{
    while (p != 0) {
        if (*p != keep)
            *p = fill;
        p++;
    }
}
