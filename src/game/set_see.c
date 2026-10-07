/* set_see: candidate for 1018:aef8 (82 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: set_see
 *
 * Writes colony c's population and its building count (colony_building_level
 * with 0) into slot `nation` of the two 4-byte (one per European nation) histories at Colony+0xba/+0xbe.
 * colony_building_level is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from colony_record_history: the Mac build's set_see(short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the first of the two functions between colony_add_unit_2 and create_colony, which are already bound, paired in order; the arity agrees and the Mac body is 1.24x this one.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

extern int colony_building_level(int colony, int bit);

void set_see(int colony, int nation)
{
    g_colonies[colony].hist_pop[nation] = g_colonies[colony].pop;
    g_colonies[colony].hist_bld[nation] = colony_building_level(colony, 0);
}
