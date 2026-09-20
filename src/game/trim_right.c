/* trim_right -- shipped at 1040:6986.
 *
 * Ported by tools/port.py from matched/game/trim_right.c.
 *
 * STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void trim_right(char *s)
{
    char *p;
    col_i16 again;

    if (*s != 0) {
        p = s + (strlen(s) - 1);
        do {
            again = 1;
            p = s + (strlen(s) - 1);
            if ((col_i16)*p == ' ' || (col_i16)*p == '\t')
                *p = 0;
            else
                again = 0;
            p--;
            if (p < s)
                again = 0;
        } while (again);
    }
}
