/* colony_building_level: candidate for 1018:3e4a (69 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Counts the set flags along a building chain: starting at b and following the signed-byte
 * link of the 12-byte building records until it goes negative.
 *
 * RENAMED 2026-09-26 from colony_building_count: the Mac build's colony_building_level(short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between building_level and top_building, which are already bound; the arity agrees and the Mac body is 1.39x this one.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int colony_has_building(int colony, int bit);
int colony_building_level(int colony, int bit)
{
    int n = 0;

    do {
        if (colony_has_building(colony, bit))
            n++;
        bit = g_buildings[bit].next_level;
    } while (bit >= 0);
    return n;
}
