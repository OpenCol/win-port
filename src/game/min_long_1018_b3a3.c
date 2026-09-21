/* min_long_1018_b3a3 -- shipped at 1018:b3a3.
 *
 * Ported by tools/port.py from matched/game/min_long_1018_b3a3.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i32 min_long_1018_b3a3(col_i32 a, col_i32 b)
{
    if (a > b)
        return b;
    else
        return a;
}
