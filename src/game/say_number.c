/* say_number -- shipped at 1040:a3a7.
 *
 * Ported by tools/port.py from matched/game/say_number.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern void LTOA1(col_i16 v, char *s, col_i16 radix);


void say_number(char *s, col_i16 v)
{
    char buf[0x14];

    LTOA1(v, buf, 10);
    strcat(s, buf);
}
