/* cut_at_newline: candidate for 1040:6902 (69 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * Terminates s at its last newline; returns where it was cut, or NULL.
 */

#include "colonization.h"

char * cut_at_newline(char *s)
{
    char *p = strrchr_rtl(s, '\n');

    if (p != 0)
        *p = 0;
    return p;
}
