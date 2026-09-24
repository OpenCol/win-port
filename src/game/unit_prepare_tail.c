/* unit_prepare_tail -- shipped at 1038:2185.
 *
 * Ported by tools/port.py from matched/game/unit_prepare_tail.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_chain_tail(col_i16 u);
void stack_reorder(col_i16 u);

col_i16 unit_prepare_tail(col_i16 u)
{
    stack_reorder(u);
    u = unit_chain_tail(u);
    return u;
}
