/* blockade_speed: candidate for 1030:758b (126 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: blockade_speed
 *
 * A per-unit distance: the type's rating plus three, doubled for type 0x10 and three more
 * for type 0x0f, less four for each item of cargo aboard, and never below one.
 *
 * RENAMED 2026-09-26 from unit_sight_or_range: the Mac build's blockade_speed(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between negotiate_europe and treaty_encounters, which are already bound; the arity agrees and the Mac body is 1.08x this one.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern unsigned char move_rate(int u);

int blockade_speed(int u)
{
    int r = move_rate(u) + 3;

    if (g_units[u].type == UNIT_PRIVATEER)
        r = r * 2;
    if (g_units[u].type == UNIT_GALLEON)
        r = r + 3;
    r -= g_units[u].cargo_count * 4;
    r = max_int_1030_ead6(1, r);
    return r;
}
