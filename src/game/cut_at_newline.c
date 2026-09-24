/* cut_at_newline -- shipped at 1040:6902.
 *
 * Ported by tools/port.py from matched/game/cut_at_newline.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * strrchr_rtl(const char *s, col_i16 c);

char * cut_at_newline(char *s)
{
    char *p = strrchr_rtl(s, '\n');

    if (p != 0)
        *p = 0;
    return p;
}
