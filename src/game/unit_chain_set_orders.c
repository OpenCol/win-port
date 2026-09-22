/* unit_chain_set_orders -- shipped at 1038:35bd.
 *
 * Ported by tools/port.py from matched/game/unit_chain_set_orders.c.
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
col_i16 unit_chain_next(col_i16 u);
col_i16 unit_chain_tail(col_i16 u);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */





void unit_chain_set_orders(col_i16 u, col_i16 v)
{
    for (u = unit_chain_tail(u); u >= 0; u = unit_chain_next(u))
        g_units[u].orders = v;
}
