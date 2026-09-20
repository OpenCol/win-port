/* max_int_1068_98e4 -- shipped at 1068:98e4.
 *
 * Ported by tools/port.py from matched/gfx/max_int_1068_98e4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 max_int_1068_98e4(col_i16 a, col_i16 b)
{
    if (a < b)
        return b;
    else
        return a;
}
