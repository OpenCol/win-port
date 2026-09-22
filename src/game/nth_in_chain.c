/* nth_in_chain -- shipped at 1038:1a02.
 *
 * Ported by tools/port.py from matched/game/nth_in_chain.c.
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

col_i16 nth_in_chain(col_i16 u, col_i16 n)
{
    register col_i16 k = -1;
    col_i16 found = -1;
    register col_i16 c;

    for (c = unit_chain_tail(u); found < 0 && c >= 0; c = unit_chain_next(c)) {
        k++;
        if (n == k)
            found = c;
    }
    return found;
}
