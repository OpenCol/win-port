/* unit_special_active: candidate for 1038:3996 (309 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * unit_is_active, except that a unit off the map still qualifies when its state (+8)
 * is 2 and its x equals the low nibble of its flags minus 20 (the Europe slot).
 *
 * RENAMED 2026-09-26 from unit_selectable_or_in_europe: the Mac build's unit_special_active(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller DoHuman; callee move_rate; callee on_map), the runner-up gets 1, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern unsigned char move_rate(int u);
int unit_special_active(int u)
{
    int r = 0;

    if (u < 0 || g_game.unit_count <= u)
        goto done;
    if (!on_map(g_units[u].x, g_units[u].y)) {
        if (g_units[u].orders != ORDERS_TRADE_ROUTE)
            goto done;
        if (g_units[u].x != (unsigned char)((g_units[u].flags & UNIT_NATION_MASK) - 20))
            goto done;
    }
    r = (unsigned char)(g_units[u].flags & UNIT_NATION_MASK) == ((unsigned char)g_game.turn_nation)
        && g_units[u].orders != ORDERS_SENTRY && g_units[u].orders != ORDERS_FORTIFIED
        && ((g_units[u].flags2 & 0x80) == 0 || g_units[u].type == UNIT_ARTILLERY)
        && move_rate(u) > g_units[u].moves_used;
done:
    return r;
}
