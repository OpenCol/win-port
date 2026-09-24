/* unit_selectable_or_in_europe -- shipped at 1038:3996.
 *
 * Ported by tools/port.py from matched/game/unit_selectable_or_in_europe.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern unsigned char far g_turn_nation;              SEG20:0x7796
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
unsigned char unit_move_points(col_i16 u);

#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */



extern col_i16 on_map(col_i16 x, col_i16 y);
/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 unit_selectable_or_in_europe(col_i16 u)
{
    col_i16 r = 0;

    if (u < 0 || g_unit_count <= u)
        goto done;
    if (!on_map(g_units[u].x, g_units[u].y)) {
        if (g_units[u].orders != ORDERS_TRADE_ROUTE)
            goto done;
        if (g_units[u].x != (unsigned char)((g_units[u].flags & UNIT_NATION_MASK) - 20))
            goto done;
    }
    r = (unsigned char)(g_units[u].flags & UNIT_NATION_MASK) == g_turn_nation
        && g_units[u].orders != ORDERS_SENTRY && g_units[u].orders != ORDERS_FORTIFIED
        && ((g_units[u].f4 & 0x80) == 0 || g_units[u].type == UNIT_ARTILLERY)
        && unit_move_points(u) > g_units[u].moves;
done:
    return r;
}
