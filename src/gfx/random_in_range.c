/* random_in_range -- shipped at 1068:b7e0.
 *
 * Ported by tools/port.py from matched/gfx/random_in_range.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 random_below(col_i16 n);

/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 random_in_range(register col_i16 lo, col_i16 hi)
{
    col_i16 span;
    col_i16 draw;

    span = hi - lo + 1;
    draw = random_below(span);
    return lo + draw;
}
