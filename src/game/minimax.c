/* minimax -- shipped at 1040:7ae2.
 *
 * Ported by tools/port.py from matched/game/minimax.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1040_aab7(col_i16 a, col_i16 b);
col_i16 min_int_1040_aadd(col_i16 a, col_i16 b);

/* same object: 4-byte `push cs; call ` (rule 47) */


col_i16 minimax(col_i16 v, col_i16 lo, col_i16 hi)
{
    v = max_int_1040_aab7(v, lo);
    v = min_int_1040_aadd(v, hi);
    return v;
}
