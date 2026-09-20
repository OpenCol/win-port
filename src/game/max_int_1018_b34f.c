/* max_int_1018_b34f -- shipped at 1018:b34f.
 *
 * Ported by tools/port.py from matched/game/max_int_1018_b34f.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 max_int_1018_b34f(col_i16 a, col_i16 b)
{
    if (a < b)
        return b;
    else
        return a;
}
