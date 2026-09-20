/* max_int_1020_a887 -- shipped at 1020:a887.
 *
 * Ported by tools/port.py from matched/game/max_int_1020_a887.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 max_int_1020_a887(col_i16 a, col_i16 b)
{
    if (a < b)
        return b;
    else
        return a;
}
