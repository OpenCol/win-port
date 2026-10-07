/* unfortify: candidate for 1030:4f7c (344 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Wakes one nation's land units that are standing next to another nation's territory: for every
 * unit of `owner` that is not a ship and whose type record's attack (+6) is more than 1, look at the
 * eight neighbouring squares, and if any of them belongs to `nation`, clear orders 5 and 6.
 *
 * Only those two orders are cleared. A unit fortified under any other code, or with no orders
 * at all, is left alone -- so this is "stop sleeping, someone is next to you" and not a general
 * wake.
 *
 * The inner loop's condition is `found == 0 && i < 8`, tested in that order, so the scan stops
 * at the first neighbouring square that matches rather than counting them.
 *
 * Three guards, all of them `continue` in the two-jump form (rule 12), and the third is the one
 * worth naming: the unit TYPE's attack byte at +6 of the record at SEG20:0x7ac8 has to be above 1.
 * Whatever that field counts, 0 and 1 are excluded from this sweep.
 *
 * `on_map` is checked after the coordinates are read and before any neighbour is looked at, so
 * a unit that is off the board -- in Europe, say -- never reaches `is_colony`.
 *
 * RENAMED 2026-09-26 from wake_units_near: the Mac build's unfortify(short, short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in negotiate_ai, negotiate_europe -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (2).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameState g_game;   /* SEG20:0x7782 */

extern int is_colony(int x, int y);

void unfortify(int nation, int owner)
{
    int found;
    int x;
    int y;
    int nx;
    int ny;
    register int i;
    register int u;

    for (u = 0; g_game.unit_count > u; u++) {
        if (NATION(g_units[u]) != (unsigned char)owner)
            continue;
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
            continue;
        if (g_unit_types[g_units[u].type].attack <= 1)
            continue;
        x = g_units[u].x;
        y = g_units[u].y;
        found = 0;
        if (on_map(x, y) == 0)
            continue;
        for (i = 0; found == 0 && i < 8; i++) {
            nx = g_dir_dx[i] + x;
            ny = g_dir_dy[i] + y;
            if (is_colony(nx, ny) == nation)
                found = 1;
        }
        if (found != 0) {
            if (g_units[u].orders == ORDERS_FORTIFY || g_units[u].orders == ORDERS_FORTIFIED)
                g_units[u].orders = ORDERS_NONE;
        }
    }
}
