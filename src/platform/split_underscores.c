/* split_underscores: candidate for 1070:387f (74 bytes).  Verdict: EXACT.
 *
 * Replaces every '_' in the string with a terminator and returns the
 * number of pieces.
 */

#include "colonization.h"

int split_underscores(char *s)
{
    char *p = s;
    int n = 1;

    for (; *p != 0; p++) {
        if ((int)*p == '_') {
            *p = 0;
            n++;
        }
    }
    return n;
}
