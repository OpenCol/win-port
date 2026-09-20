/* min_char_ptr_1020_b2f3 -- shipped at 1020:b2f3.
 *
 * Ported by tools/port.py from matched/game/min_char_ptr_1020_b2f3.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char * min_char_ptr_1020_b2f3(char *a, char *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
