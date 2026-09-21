/* max_long_1048_dd87 -- shipped at 1048:dd87.
 *
 * Ported by tools/port.py from matched/game/max_long_1048_dd87.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i32 max_long_1048_dd87(col_i32 a, col_i32 b)
{
    if (a < b)
        return b;
    else
        return a;
}
