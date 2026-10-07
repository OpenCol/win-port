/* append_newline: candidate for 1040:6947 (63 bytes).  Verdict: RELOCATABLE.
 *
 * Appends a newline and terminator to a string.
 */

#include "colonization.h"

void append_newline(char *s)
{
    char *p = s + STRLEN(s);

    *p = '\n';
    p++;
    *p = 0;
}
