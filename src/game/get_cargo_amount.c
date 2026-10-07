/* get_cargo_amount: candidate for 1018:7c5a (39 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Returns the amount held in cargo slot `slot` of unit `u` (the byte array at
 * Unit+0x10; the slot's goods type lives in the nibble array at +0xd).
 *
 * RENAMED 2026-09-26 from unit_cargo_amount: the Mac build's get_cargo_amount(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller DrawColonyShips; caller DrawEuropeShips; caller InfoCargo; caller ReportsShip; caller capture_cargo), the runner-up gets 3, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

int get_cargo_amount(int u, int slot)
{
    return g_units[u].hold.amount[slot];
}
