/* move_to_top: candidate for 1038:1dbb (63 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Calls the position handler for unit n with its own x and y (bytes +0 and +1).
 *
 * RENAMED 2026-09-26 from unit_replace_at_self: the Mac build's move_to_top(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller ColonyClickUnits; caller colony_click_ship; caller combat_fight; caller europe_click_ship; caller europe_click_unit), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void relocate_unit(int unit, int x, int y);
void move_to_top(int unit)
{
    int u = unit;

    relocate_unit(u, g_units[u].x, g_units[u].y);
}
