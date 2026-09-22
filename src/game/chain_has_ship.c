/* chain_has_ship -- shipped at 1038:3553.
 *
 * Ported by tools/port.py from matched/game/chain_has_ship.c.
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

#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */





col_i16 chain_has_ship(col_i16 u)
{
    col_i16 r = 0;

    for (u = unit_chain_tail(u); u >= 0 && r == 0; u = unit_chain_next(u))
        r |= g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST;
    return r;
}
