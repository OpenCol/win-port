/* unit_chain_set_nation -- shipped at 1038:2795.
 *
 * Ported by tools/port.py from matched/game/unit_chain_set_nation.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_chain_next(col_i16 u);
col_i16 unit_chain_tail(col_i16 u);
void unit_nation_flag_set(col_i16 u, col_i16 n);

void unit_chain_set_nation(col_i16 u, col_i16 n)
{
    u = unit_chain_tail(u);
    if (u < 0)
        goto done;
    do {
        unit_nation_flag_set(u, n);
        u = unit_chain_next(u);
    } while (u >= 0);
done:
    ;
}
