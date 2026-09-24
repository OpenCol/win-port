/* say_long -- shipped at 1040:a44c.
 *
 * Ported by tools/port.py from matched/game/say_long.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern void ltoa_rtl(col_i32 v, char *s, col_i16 radix);


void say_long(char *s, col_i32 v)
{
    char buf[0x14];

    ltoa_rtl(v, buf, 10);
    strcat(s, buf);
}
