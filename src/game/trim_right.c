/* trim_right: candidate for 1040:6986 (140 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * Strips trailing spaces and tabs: repeatedly finds the last character, clears it when
 * blank, and stops at the first non-blank or when the pointer passes the start.
 */

#include "colonization.h"

void trim_right(char *s)
{
    char *p;
    int again;

    if (*s != 0) {
        p = s + (STRLEN(s) - 1);
        do {
            again = 1;
            p = s + (STRLEN(s) - 1);
            if ((int)*p == ' ' || (int)*p == '\t')
                *p = 0;
            else
                again = 0;
            p--;
            if (p < s)
                again = 0;
        } while (again);
    }
}
