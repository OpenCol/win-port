/* append_number_padded: candidate for 1040:5fae (177 bytes, 2 callers).  Verdict: RELOCATABLE.
 *
 * Appends `digits` zero characters to s, then writes value into them by repeated
 * division: for each position from the left, the quotient by the current power of ten is
 * added to the '0' and subtracted out. Returns s.
 */

#include "colonization.h"

char * append_number_padded(char *s, int value, int digits)
{
    int i;
    int pow = 1;
    int digit;

    for (i = 0; i < digits; i++) {
        if (i > 0)
            pow *= 10;
        STRCAT(s, "0");
    }
    i = STRLEN(s) - digits;
    while (STRLEN(s) > i) {
        if (value >= pow) {
            digit = value / pow;
            value = value - digit * pow;
            s[i] += digit;
        }
        pow = pow / 10;
        i++;
    }
    return s;
}
