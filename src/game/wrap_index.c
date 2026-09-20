/* wrap_index -- shipped at 1040:7d49.
 *
 * Ported by tools/port.py from matched/game/wrap_index.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 wrap_index(col_i16 i, col_i16 n)
{
    if (i < 0)
        i = n - 1;
    if (i >= n)
        i = 0;
    return i;
}
