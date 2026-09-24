/* unit_stack_each -- shipped at 1020:345a.
 *
 * Ported by tools/port.py from matched/game/unit_stack_each.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 resolve_combat_loss(col_i16 u, col_i16 a, col_i16 b, col_i16 c, col_i16 d);

extern col_i16 unit_chain_tail(col_i16 u);
extern col_i16 unit_chain_next(col_i16 u);

/* same object: 4-byte `push cs; call ` */


void unit_stack_each(col_i16 u, col_i16 a, col_i16 b, col_i16 c, col_i16 d)
{
    col_i16 n;

    n = unit_chain_tail(u);
    for (u = n; u >= 0; u = n) {
        n = unit_chain_next(u);
        if (resolve_combat_loss(u, a, b, c, d)) {
            if (n > u)
                n--;
            if (a > u)
                a--;
        }
    }
}
