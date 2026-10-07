/* pascal_to_c: candidate for 1040:77ca (62 bytes).  Verdict: EXACT.
 *
 * Turns a length-prefixed string into a C string in place: the bytes move
 * down one and a terminator follows.
 */

#include "colonization.h"

void pascal_to_c(char *s)
{
    unsigned char n = s[0];
    int i;

    for (i = 0; n > i; i++)
        s[i] = s[i + 1];
    s[i] = 0;
}
