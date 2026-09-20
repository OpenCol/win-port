/* max_int_1058_c226 -- shipped at 1058:c226.
 *
 * Ported by tools/port.py from matched/game/max_int_1058_c226.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 max_int_1058_c226(col_i16 a, col_i16 b)
{
    if (a < b)
        return b;
    else
        return a;
}
