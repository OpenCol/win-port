/* max_char_ptr -- shipped at 1030:f4d8.
 *
 * Ported by tools/port.py from matched/game/max_char_ptr.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char * max_char_ptr(char *a, char *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
