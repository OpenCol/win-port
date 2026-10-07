/* to_pascal_string: candidate for 1040:7808 (106 bytes).  Verdict: RELOCATABLE.
 *
 * Converts a C string in place to a length-prefixed one through a 256-byte buffer.
 */

#include "colonization.h"

void to_pascal_string(char *s)
{
    char *p;
    char buf[0x100];
    int i;

    buf[0] = STRLEN(s);
    p = s;
    i = 1;
    while (*p != 0) {
        buf[i] = *p;
        p++;
        i++;
    }
    memcpy_rtl(s, buf, buf[0] + 1);
}
