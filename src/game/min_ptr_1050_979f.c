/* min_ptr_1050_979f -- shipped at 1050:979f.
 *
 * Ported by tools/port.py from matched/game/min_ptr_1050_979f.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 * min_ptr_1050_979f(col_i16 *a, col_i16 *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
