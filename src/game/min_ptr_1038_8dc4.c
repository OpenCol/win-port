/* min_ptr_1038_8dc4 -- shipped at 1038:8dc4.
 *
 * Ported by tools/port.py from matched/game/min_ptr_1038_8dc4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 * min_ptr_1038_8dc4(col_i16 *a, col_i16 *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
