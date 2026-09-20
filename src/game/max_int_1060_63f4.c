/* max_int_1060_63f4 -- shipped at 1060:63f4.
 *
 * Ported by tools/port.py from matched/game/max_int_1060_63f4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 max_int_1060_63f4(col_i16 a, col_i16 b)
{
    if (a < b)
        return b;
    else
        return a;
}
