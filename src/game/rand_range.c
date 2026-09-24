/* rand_range -- shipped at 1040:751a.
 *
 * Ported by tools/port.py from matched/game/rand_range.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 rand_rtl(void);

col_i16 rand_range(col_i16 lo, col_i16 hi)
{
    col_i16 span = hi - lo + 1;
    col_i16 r;

    r = (col_i16)((col_i32)rand_rtl() * span >> 15);
    return r + lo;
}
