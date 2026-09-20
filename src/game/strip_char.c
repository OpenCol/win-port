/* strip_char -- shipped at 1040:60a3.
 *
 * Ported by tools/port.py from matched/game/strip_char.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void strip_char(char *s, char c)
{
    char *p;
    char *q;

    for (p = s; *p != 0; p++) {
        if (*p == c) {
            for (q = p; *q != 0; q++)
                q[0] = q[1];
        }
    }
}
