/* unspot_unit: candidate for 1038:270a (37 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Clears the high nibble of the unit's flags byte (Unit+3), keeping the
 * nation in the low nibble.  Nothing for a negative unit.
 *
 * RENAMED 2026-09-26 from unit_clear_flags_high: the Mac build's unspot_unit(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the first of the two functions between unit_activate and spot_unit, which are already bound, paired in order; the arity agrees and the Mac body is 1.41x this one.
 */

#include "colonization.h"

#include "records.h"

void unspot_unit(int u)
{
    if (u >= 0)
        g_units[u].flags &= 0xf;
}
