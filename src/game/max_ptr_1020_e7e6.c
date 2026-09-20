/* max_ptr_1020_e7e6 -- shipped at 1020:e7e6.
 *
 * Ported by tools/port.py from matched/game/max_ptr_1020_e7e6.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 * max_ptr_1020_e7e6(col_i16 *a, col_i16 *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
