/* min_int_1010_e5d0 -- shipped at 1010:e5d0.
 *
 * Ported by tools/port.py from matched/game/min_int_1010_e5d0.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 min_int_1010_e5d0(col_i16 a, col_i16 b)
{
    if (a > b)
        return b;
    else
        return a;
}
