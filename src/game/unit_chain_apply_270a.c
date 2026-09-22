/* unit_chain_apply_270a -- shipped at 1038:272f.
 *
 * Ported by tools/port.py from matched/game/unit_chain_apply_270a.c.
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
void unit_clear_flags_high(col_i16 u);

void unit_chain_apply_270a(col_i16 u)
{
    u = unit_chain_tail(u);
    if (u < 0)
        goto done;
    do {
        unit_clear_flags_high(u);
        u = unit_chain_next(u);
    } while (u >= 0);
done:
    ;
}
