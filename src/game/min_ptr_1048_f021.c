/* min_ptr_1048_f021 -- shipped at 1048:f021.
 *
 * Ported by tools/port.py from matched/game/min_ptr_1048_f021.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 * min_ptr_1048_f021(col_i16 *a, col_i16 *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
