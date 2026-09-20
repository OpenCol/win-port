/* min_ptr_1028_e5cd -- shipped at 1028:e5cd.
 *
 * Ported by tools/port.py from matched/game/min_ptr_1028_e5cd.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 * min_ptr_1028_e5cd(col_i16 *a, col_i16 *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
