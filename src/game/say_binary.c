/* say_binary -- shipped at 1040:a3e0.
 *
 * Ported by tools/port.py from matched/game/say_binary.c.
 *
 * STRCAT -> strcat, STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char g_zero_digit2[];                         DGROUP:0x1fba
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern void LTOA1(col_i16 v, char *s, col_i16 radix);




void say_binary(char *s, col_i16 v)
{
    char buf[0x14];
    col_i16 i;
    col_i16 len;

    LTOA1(v, buf, 2);
    len = strlen(buf);
    for (i = 0; 8 - len > i; i++)
        strcat(s, g_zero_digit2);
    strcat(s, buf);
}
