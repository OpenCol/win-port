/* max_int_1038_e46c -- shipped at 1038:e46c.
 *
 * Ported by tools/port.py from matched/game/max_int_1038_e46c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 max_int_1038_e46c(col_i16 a, col_i16 b)
{
    if (a < b)
        return b;
    else
        return a;
}
