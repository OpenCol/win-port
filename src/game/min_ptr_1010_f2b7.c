/* min_ptr_1010_f2b7 -- shipped at 1010:f2b7.
 *
 * Ported by tools/port.py from matched/game/min_ptr_1010_f2b7.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 * min_ptr_1010_f2b7(col_i16 *a, col_i16 *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
