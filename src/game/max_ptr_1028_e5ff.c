/* max_ptr_1028_e5ff -- shipped at 1028:e5ff.
 *
 * Ported by tools/port.py from matched/game/max_ptr_1028_e5ff.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 * max_ptr_1028_e5ff(col_i16 *a, col_i16 *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
