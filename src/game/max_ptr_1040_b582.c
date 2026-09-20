/* max_ptr_1040_b582 -- shipped at 1040:b582.
 *
 * Ported by tools/port.py from matched/game/max_ptr_1040_b582.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 * max_ptr_1040_b582(col_i16 *a, col_i16 *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
