/* max_ptr_1018_b55a -- shipped at 1018:b55a.
 *
 * Ported by tools/port.py from matched/game/max_ptr_1018_b55a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 * max_ptr_1018_b55a(col_i16 *a, col_i16 *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
