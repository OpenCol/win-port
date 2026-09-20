/* nation_pair_flags_or_both -- shipped at 1030:7ed7.
 *
 * Ported by tools/port.py from matched/game/nation_pair_flags_or_both.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 nation_pair_flags_get(col_i16 a, col_i16 b);
col_i16 nation_pair_flags_set(col_i16 a, col_i16 b, col_i16 v);

/* same object: 4-byte `push cs; call ` (rule 47) */


col_i16 nation_pair_flags_or_both(col_i16 a, col_i16 b, col_i16 mask)
{
    col_i16 r1, r2;

    r1 = nation_pair_flags_set(a, b, nation_pair_flags_get(a, b) | mask);
    r2 = nation_pair_flags_set(b, a, nation_pair_flags_get(b, a) | mask);
    if ((r1 & mask) != (r2 & mask))
        ;
    return r1;
}
