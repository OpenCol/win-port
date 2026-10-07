/* unit_is_active: candidate for 1038:3800 (236 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Whether unit u can be picked: a valid index on the map, owned by the player
 * (SEG20:0x7796), state (+8) neither 1 nor 6, not (bit 0x80 of +4 with a type other than
 * 11), and move_rate above the byte at +5.
 *
 * RENAMED 2026-09-26 from unit_selectable: the Mac build's unit_is_active(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller colony_click_ship; caller move_unit; caller orders_activate; callee on_map), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern unsigned char move_rate(int u);
int unit_is_active(int u)
{
    int r = 0;

    if (u < 0 || g_game.unit_count <= u)
        goto done;
    if (!on_map(g_units[u].x, g_units[u].y))
        goto done;
    r = (unsigned char)(g_units[u].flags & UNIT_NATION_MASK) == ((unsigned char)g_game.turn_nation)
        && g_units[u].orders != ORDERS_SENTRY && g_units[u].orders != ORDERS_FORTIFIED
        && ((g_units[u].flags2 & 0x80) == 0 || g_units[u].type == UNIT_ARTILLERY)
        && move_rate(u) > g_units[u].moves_used;
done:
    return r;
}
