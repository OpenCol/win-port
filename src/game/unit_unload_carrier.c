/* unit_unload_carrier -- shipped at 1038:1dfa.
 *
 * Ported by tools/port.py from matched/game/unit_unload_carrier.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int g_stub_a;                                 
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_chain26_tail(col_i16 u);
col_i16 unit_chain_next(col_i16 u);
col_i16 unit_chain_tail(col_i16 u);
void unit_unlink(col_i16 u);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */








void unit_unload_carrier(col_i16 u)
{
    col_i16 p;
    col_i16 x;
    col_i16 y;
    col_i16 t;

    if (g_units[u].link2 >= 0) {
        x = g_units[u].x;
        y = g_units[u].y;
        p = unit_chain_tail(u);
        if (p == u)
            p = unit_chain_next(u);
        unit_unlink(u);
        t = unit_chain26_tail(p);
        g_units[t].link2 = u;
        g_units[u].link = t;
        g_units[u].link2 = -1;
        g_units[u].x = x;
        g_units[u].y = y;
    }
}
