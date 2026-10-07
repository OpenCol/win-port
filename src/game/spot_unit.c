/* spot_unit: candidate for 1038:2768 (45 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Marks the unit as SPOTTED BY nation n: sets bit 4+n of its flag byte (+3), whose high nibble is
 * one bit per European nation, laid out like the map's plane 3. The drawing code will not put a
 * unit on screen unless the viewer's bit is set or reveal-all is on, so this is what makes a unit
 * appear. Callers are the moments a nation learns a unit exists -- `colony_field_status` for the
 * units standing in a new colony's tiles, `allied_invasion` and `king_invasion`
 * for the units they create. Negative unit indices are ignored.
 *
 * RENAMED 2026-09-26 from unit_nation_flag_set: the Mac build's spot_unit(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller allied_invasion; caller colony_field_status; caller indian_move; caller king_invasion), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

#include "units.h"

#include "records.h"

void spot_unit(int u, int n)
{
    if (u >= 0)
        g_units[u].flags |= (unsigned char)UNIT_SEEN_BY(n);
}
