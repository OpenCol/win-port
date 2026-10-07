/* needs_a_building: candidate for 1018:038f (169 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Whether the colony has fewer levels of the building in row idx of the
 * 4-byte table at SEG20:0xdca8 than its goods call for: 2 levels once the
 * SEG21:0x122 figure for the goods reaches 3, 3 levels at 8 or when the
 * stock is 100 or more.
 *
 * RENAMED 2026-09-26 from colony_needs_building: the Mac build's needs_a_building(short), bound by call-sequence alignment: in colony_plan the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (1), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "records.h"

extern int building_level(int bit);

int needs_a_building(int idx)
{
    /* declared HERE, at block scope: this function reaches them with a segment
       fixup while the rest of segment 4 reads the same globals through DS, and one
       C++ module can hold both views only this way */

    register int need = 0;
    register int goods = g_build_needs[idx].goods;
    int building = g_build_needs[idx].building;
    int have = building_level(building);

    if ((unsigned)g_prod[goods] >= 3)
        need = 2;
    if ((unsigned)g_prod[goods] >= 8)
        need = 3;
    if (g_cur_colony_rec->stock[goods] >= 100)
        need = 3;
    return have < need;
}
