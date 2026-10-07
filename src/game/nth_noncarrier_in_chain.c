/* nth_noncarrier_in_chain: candidate for 1038:1a89 (124 bytes).  Verdict: RELOCATABLE.
 * symbol: nth_noncarrier_in_chain
 *
 * The n-th unit (0-based) of u's chain whose type has no hold
 * (UnitType+7 == 0), or -1.  Both chain helpers are -called and
 * defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from nth_carrier_in_chain: The loop SKIPS every unit whose type has holds > 0 (`if (holds > 0) continue;`) and counts only the units with no hold, returning the n-th of those or -1. Both callers (ColonyWin_ColonyMouseUnits, ColonyWin_ColonyClickUnits) clamp the index against g_tile_noncarriers. The old name says the opposite: it returns a non-carrier, never a carrier.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;
extern int top_unit(int u);
extern int next_unit(int u);

int nth_noncarrier_in_chain(int u, int n)
{
    register int k = -1;
    int found = -1;
    register int unit;

    for (unit = top_unit(u); found < 0 && unit >= 0; unit = next_unit(unit)) {
        if (g_unit_types[g_units[unit].type].holds > 0)
            continue;
        k++;
        if (n == k)
            found = unit;
    }
    return found;
}
