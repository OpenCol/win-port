/* min_ptr_1030_f474 -- shipped at 1030:f474.
 *
 * Ported by tools/port.py from matched/game/min_ptr_1030_f474.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 * min_ptr_1030_f474(col_i16 *a, col_i16 *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
