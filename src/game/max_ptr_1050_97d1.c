/* max_ptr_1050_97d1 -- shipped at 1050:97d1.
 *
 * Ported by tools/port.py from matched/game/max_ptr_1050_97d1.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 * max_ptr_1050_97d1(col_i16 *a, col_i16 *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
