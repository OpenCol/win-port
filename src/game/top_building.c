/* top_building: candidate for 1018:3e8f (63 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Follows the signed-byte link at +4 of the 12-byte building records until it goes
 * negative and returns the last index.
 *
 * RENAMED 2026-09-26 from building_line_tail: the Mac build's top_building(short), bound by call-sequence alignment: in colony_plan the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (1), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "records.h"

int top_building(int building)
{
    while ((int)g_buildings[building].next_level >= 0)
        building = g_buildings[building].next_level;
    return building;
}
