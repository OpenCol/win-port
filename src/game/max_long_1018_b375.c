/* max_long_1018_b375 -- shipped at 1018:b375.
 *
 * Ported by tools/port.py from matched/game/max_long_1018_b375.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i32 max_long_1018_b375(col_i32 a, col_i32 b)
{
    if (a < b)
        return b;
    else
        return a;
}
