/* unit_chain_free_all -- shipped at 1038:2fdd.
 *
 * Ported by tools/port.py from matched/game/unit_chain_free_all.c.
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
void unit_record_free(col_i16 u);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */




void unit_chain_free_all(col_i16 u)
{
    col_i16 nx;

    u = unit_chain_tail(u);
    while (u >= 0) {
        nx = unit_chain_next(u);
        unit_record_free(u);
        if (nx > u)
            nx--;
        u = nx;
    }
}
