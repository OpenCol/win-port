/* strip_char: candidate for 1040:60a3 (88 bytes).  Verdict: EXACT.
 *
 * Removes occurrences of c from a string by shifting the tail down one at
 * each hit (a run of two is only half removed: the scan moves on).
 */

#include "colonization.h"

void strip_char(char *s, char ch)
{
    char *p;
    char *q;

    for (p = s; *p != 0; p++) {
        if (*p == ch) {
            for (q = p; *q != 0; q++)
                q[0] = q[1];
        }
    }
}
