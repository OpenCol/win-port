/* get_next_active_unit: candidate for 1038:3acb (131 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: get_next_active_unit
 *
 * The next unit after c (wrapping) that unit_special_active
 * accepts; from an out-of-range c the search starts at 0 and may end on
 * the last unit.  -1 when none qualifies or there are no units.  The
 * test is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from next_selectable_unit: the Mac build's get_next_active_unit(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the first of the two functions between unit_special_active and end_unit_turn, which are already bound, paired in order; the arity agrees and the Mac body is 0.92x this one.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern int g_stub_a;

extern int unit_special_active(int u);

int get_next_active_unit(int unit)
{
    int r;
    int last;

    if (g_game.unit_count <= unit || unit < 0) {
        unit = -1;
        last = g_game.unit_count - 1;
        if (g_game.unit_count == 0)
            goto done;
    } else {
        last = unit;
    }
    do {
        unit++;
        if (g_game.unit_count <= unit)
            unit = 0;
        r = unit_special_active(unit);
    } while (r == 0 && unit != last);
    if (r == 0)
        unit = -1;
done:
    return unit;
}
