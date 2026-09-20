/* max_ptr_1038_8d92 -- shipped at 1038:8d92.
 *
 * Ported by tools/port.py from matched/game/max_ptr_1038_8d92.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 * max_ptr_1038_8d92(col_i16 *a, col_i16 *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
