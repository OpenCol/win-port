/* min_long_1020_a997 -- shipped at 1020:a997.
 *
 * Ported by tools/port.py from matched/game/min_long_1020_a997.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i32 min_long_1020_a997(col_i32 a, col_i32 b)
{
    if (a > b)
        return b;
    else
        return a;
}
