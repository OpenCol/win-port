/* pick_ship_in_chain -- shipped at 1040:1f49.
 *
 * Ported by tools/port.py from matched/game/pick_ship_in_chain.c.
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
col_i16 unit_portrait(col_i16 u);

#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */


extern col_i16 unit_chain_tail(col_i16 u);
extern col_i16 unit_chain_next(col_i16 u);



col_i16 pick_ship_in_chain(col_i16 u, col_i16 flag, col_i16 *out)
{
    register col_i16 c;
    register col_i16 r;

    if (flag != 0) {
        r = -1;
        for (c = unit_chain_tail(u); r < 0 && c >= 0; c = unit_chain_next(c)) {
            if (g_units[c].type >= UNIT_SHIP_FIRST && g_units[c].type <= UNIT_SHIP_LAST)
                r = c;
        }
        if (r < 0)
            r = u;
    } else {
        r = u;
    }
    *out = r;
    return unit_portrait(r);
}
