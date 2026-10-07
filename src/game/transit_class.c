/* transit_class: candidate for 1030:2fd9 (177 bytes).  Verdict: RELOCATABLE.
 * symbol: transit_class
 * name-source: sibling-build
 *
 * One turn of a crossing. Every unit sitting on the pseudo-square `nation + from` has the
 * counter `set_course_for_new_world` gave it decremented, and the ones that reach zero are
 * relinked to `nation + to` and unloaded.
 *
 * The chain's next pointer is taken BEFORE the unit is moved, which is what makes moving a unit
 * out of the chain safe while walking it.
 *
 * The two callers pass the offsets in both directions, so this one function runs the crossing
 * out to Europe and the crossing back.
 *
 * The counter is an unsigned byte and the test is `> 0` (`JNA`): a unit already at zero is moved
 * again every turn rather than being skipped, which is harmless because the relink is idempotent
 * once it has arrived.
 *
 * RENAMED 2026-09-26 from sail_countdown: the Mac build's transit_class(short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between set_course_for_new_world and square_ok, which are already bound; the arity agrees and the Mac body is 1.04x this one.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);
extern void relocate_unit(int u, int x, int y);
extern void move_to_bottom(int u);

void transit_class(int from, register int to)
{
    int next;
    register int u;

    u = top_unit_in_square(g_game.turn_nation + from, g_game.turn_nation + from);
    while (u >= 0) {
        next = next_unit(u);
        if (g_units[u].counter > 0)
            g_units[u].counter--;
        if (g_units[u].counter == 0) {
            relocate_unit(u, g_game.turn_nation + to, g_game.turn_nation + to);
            move_to_bottom(u);
        }
        u = next;
    }
}
