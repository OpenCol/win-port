/* unit_settle_or_move -- shipped at 1038:35fc.
 *
 * Ported by tools/port.py from matched/game/unit_settle_or_move.c.
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
void ship_take_aboard(col_i16 u);
void unit_detach_attach(col_i16 u, col_i16 a, col_i16 b);

#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */






void unit_settle_or_move(col_i16 u)
{
    if (u < 0)
        goto done;
    if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
        ship_take_aboard(u);
    else
        unit_detach_attach(u, -2, -2);
done:
    ;
}
