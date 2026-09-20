/* append_newline -- shipped at 1040:6947.
 *
 * Ported by tools/port.py from matched/game/append_newline.c.
 *
 * STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void append_newline(char *s)
{
    char *p = s + strlen(s);

    *p = '\n';
    p++;
    *p = 0;
}
