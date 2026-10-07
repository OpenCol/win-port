/* kill_strategy: candidate for 1028:0042 (186 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Empties every entry of row `row` of the 16-entry table at SEG24:0x400 whose third byte is
 * `kind` and whose square lies within `radius` of (x, y), by octile distance. "Empty" is
 * `kind = -1, priority = 0`, the same pair `clear_strategy_row` writes and `add_plan`
 * looks for -- so a cleared entry is reusable rather than merely blank.
 *
 * `clear_plan` is the other half of this table's maintenance: this forgets entries,
 * that one replays the survivors into the 64-entry table.
 *
 * The kind test stays 8-bit -- `CMP AL,[bp+8]` with no promotion on either side -- so the
 * parameter is an `int` -- the callers widen it (`CBW`, `MOV AH,0`) before the push, as the
 * module rebuild showed -- and the compare casts it: `kind == (signed char)kind`. A `char`
 * parameter compiles the same callee as C, but widens as C++ and drops the callers' widening.
 *
 * RENAMED 2026-09-26 from clear_cells_near: the Mac build's kill_strategy(short x5): (nation, kind, x, y, radius) empties every strategy entry of that kind within the radius -- the only five-argument remover. The Mac build's AI list family in its own module: add_/push_/kill_/at_plan over one per-nation table, add_/push_/kill_strategy over a second, add_/push_/clear_pickup over the cargo list. The Windows module 1028:0000..06cd holds the same three families side by side, and the pickup trio -- whose Windows names were already settled from behaviour -- pairs one-for-one by arity (4, 1, 0), which fixes what push means here: open a gap by shifting up.
 */

#include "colonization.h"

#include "records.h"

extern int map_dist(int x1, int y1, int x2, int y2);

void kill_strategy(int nation, int kind, int x, int y, int radius)
{
    int dist;
    register int i;

    for (i = 0; i < 0x10; i++) {
        if (g_strategies[nation][i].kind == (signed char)kind) {
            dist = map_dist(x, y, g_strategies[nation][i].x, g_strategies[nation][i].y);
            if (dist <= radius) {
                g_strategies[nation][i].kind = 0xff;
                g_strategies[nation][i].priority = 0;
            }
        }
    }
}
