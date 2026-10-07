/* pick_ship_in_chain: candidate for 1040:1f49 (131 bytes).  Verdict: RELOCATABLE.
 *
 * size-source: 131, not the census 125. The trailing 6 bytes are a SECOND epilogue, and
 * they are there because the body ends in an explicit `return;` -- Borland emits an
 * epilogue for the return statement AND one for the end of the function (rule 44).
 * symbol: pick_ship_in_chain
 *
 * With `flag` set, the first ship (type 13..18) in u's chain, or u itself
 * when there is none; without it u.  The choice goes to *out and to
 * UnitIcon (-called, defined in-file as a stub).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int g_stub_a;
extern int top_unit(int u);
extern int next_unit(int u);

extern int UnitIcon(int u);

int pick_ship_in_chain(int u, int flag, int *out)
{
    register int unit;
    register int r;

    if (flag != 0) {
        r = -1;
        for (unit = top_unit(u); r < 0 && unit >= 0; unit = next_unit(unit)) {
            if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST)
                r = unit;
        }
        if (r < 0)
            r = u;
    } else {
        r = u;
    }
    *out = r;
    return UnitIcon(r);
}
