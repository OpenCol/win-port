/* get_cargo: candidate for 1018:7be8 (114 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Cargo slot contents: -1 when the slot is at or past the unit's cargo count (+0xc), else
 * the nibble of byte slot/2 in the cargo array (+0xd): high nibble for odd slots, low for
 * even.
 *
 * RENAMED 2026-09-26 from unit_cargo_type: the Mac build's get_cargo(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller DrawColonyShips; caller DrawEuropeShips; caller DrawLandUnit; caller InfoCargo; caller ReportsShip), the runner-up gets 5, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

int get_cargo(int u, int slot)
{
    int contents;
    int idx;

    if (g_units[u].cargo_count <= slot) {
        contents = -1;
    } else {
        idx = slot >> 1;
        contents = g_units[u].cargo[idx];
        if (slot & 1)
            contents = contents >> 4;
        else
            contents = contents & 0xf;
    }
    return contents;
}
