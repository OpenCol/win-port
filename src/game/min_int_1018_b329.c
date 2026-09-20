/* min_int_1018_b329 -- shipped at 1018:b329.
 *
 * Ported by tools/port.py from matched/game/min_int_1018_b329.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 min_int_1018_b329(col_i16 a, col_i16 b)
{
    if (a > b)
        return b;
    else
        return a;
}
