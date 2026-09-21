/* min_long_1070_de2b -- shipped at 1070:de2b.
 *
 * Ported by tools/port.py from matched/platform/min_long_1070_de2b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i32 min_long_1070_de2b(col_i32 a, col_i32 b)
{
    if (a > b)
        return b;
    else
        return a;
}
