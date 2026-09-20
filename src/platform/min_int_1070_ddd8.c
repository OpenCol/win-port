/* min_int_1070_ddd8 -- shipped at 1070:ddd8.
 *
 * Ported by tools/port.py from matched/platform/min_int_1070_ddd8.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 min_int_1070_ddd8(col_i16 a, col_i16 b)
{
    if (a > b)
        return b;
    else
        return a;
}
