/* min_long_1028_e39c -- shipped at 1028:e39c.
 *
 * Ported by tools/port.py from matched/game/min_long_1028_e39c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i32 min_long_1028_e39c(col_i32 a, col_i32 b)
{
    if (a > b)
        return b;
    else
        return a;
}
