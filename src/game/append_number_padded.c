/* append_number_padded -- shipped at 1040:5fae.
 *
 * Ported by tools/port.py from matched/game/append_number_padded.c.
 *
 * STRCAT -> strcat, STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char g_zero_digit[];                          DGROUP:0x1d5b
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

char * append_number_padded(char *s, col_i16 value, col_i16 digits)
{
    col_i16 i;
    col_i16 pow = 1;
    col_i16 q;

    for (i = 0; i < digits; i++) {
        if (i > 0)
            pow *= 10;
        strcat(s, g_zero_digit);
    }
    i = strlen(s) - digits;
    while (strlen(s) > i) {
        if (value >= pow) {
            q = value / pow;
            value = value - q * pow;
            s[i] += q;
        }
        pow = pow / 10;
        i++;
    }
    return s;
}
