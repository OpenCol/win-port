/* max_ptr_1048_efef -- shipped at 1048:efef.
 *
 * Ported by tools/port.py from matched/game/max_ptr_1048_efef.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 * max_ptr_1048_efef(col_i16 *a, col_i16 *b)
{
    if (*a > *b)
        return a;
    else
        return b;
}
