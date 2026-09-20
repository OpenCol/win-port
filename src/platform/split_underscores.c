/* split_underscores -- shipped at 1070:387f.
 *
 * Ported by tools/port.py from matched/platform/split_underscores.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 split_underscores(char *s)
{
    char *p = s;
    col_i16 n = 1;

    for (; *p != 0; p++) {
        if ((col_i16)*p == '_') {
            *p = 0;
            n++;
        }
    }
    return n;
}
