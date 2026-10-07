/* say_binary: candidate for 1040:a3e0 (108 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * Appends an int in base 2, zero-padded to eight digits.
 */

#include "colonization.h"

void say_binary(char *s, int value)
{
    char buf[0x14];
    int i;
    int len;

    LTOA1(value, buf, 2);
    len = STRLEN(buf);
    for (i = 0; 8 - len > i; i++)
        STRCAT(s, "0");
    STRCAT(s, buf);
}
