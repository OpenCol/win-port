/* min_char_ptr_1030_f50a -- shipped at 1030:f50a.
 *
 * Ported by tools/port.py from matched/game/min_char_ptr_1030_f50a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char * min_char_ptr_1030_f50a(char *a, char *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
