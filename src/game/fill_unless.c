/* fill_unless -- shipped at 1040:6075.
 *
 * Ported by tools/port.py from matched/game/fill_unless.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void fill_unless(char *p, char keep, char fill)
{
    while (p != 0) {
        if (*p != keep)
            *p = fill;
        p++;
    }
}
