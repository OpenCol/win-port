/* say_long: candidate for 1040:a44c (58 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * Appends a long in base 10 to a string.
 */

#include "colonization.h"

void say_long(char *s, long amount)
{
    char buf[0x14];

    ltoa_rtl(amount, buf, 10);
    STRCAT(s, buf);
}
