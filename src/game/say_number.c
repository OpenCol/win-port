/* say_number: candidate for 1040:a3a7 (57 bytes, 142 callers).  Verdict: RELOCATABLE.
 *
 * Appends an int in base 10 to a string.
 */

#include "colonization.h"

void say_number(char *s, int number)
{
    char buf[0x14];

    LTOA1(number, buf, 10);
    STRCAT(s, buf);
}
