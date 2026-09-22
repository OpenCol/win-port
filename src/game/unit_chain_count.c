/* unit_chain_count -- shipped at 1038:1b05.
 *
 * Ported by tools/port.py from matched/game/unit_chain_count.c.
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

col_i16 unit_chain_count(col_i16 u)
{
    col_i16 n = 0;

    for (u = unit_chain_tail(u); u >= 0; u = unit_chain_next(u))
        n++;
    return n;
}
