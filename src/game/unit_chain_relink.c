/* unit_chain_relink -- shipped at 1038:1ed3.
 *
 * Ported by tools/port.py from matched/game/unit_chain_relink.c.
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
void unit_detach_attach(col_i16 u, col_i16 a, col_i16 b);

void unit_chain_relink(col_i16 u, col_i16 a, col_i16 b)
{
    register col_i16 c;
    register col_i16 nx;

    c = unit_chain_tail(u);
    while (c >= 0) {
        nx = unit_chain_next(c);
        unit_detach_attach(c, a, b);
        c = nx;
    }
}
