/* min_int_1038_8680 -- shipped at 1038:8680.
 *
 * Ported by tools/port.py from matched/game/min_int_1038_8680.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 min_int_1038_8680(col_i16 a, col_i16 b)
{
    if (a > b)
        return b;
    else
        return a;
}
