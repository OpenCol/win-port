/* can_see: candidate for 1018:af4a (83 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * 1 when colony c belongs to `nation`, when the reveal-all word at SEG20:0x77a4 is
 * set, or when the colony's history slot for that nation (Colony+0xba)
 * is non-zero.
 *
 * RENAMED 2026-09-26 from colony_known_to: the Mac build's can_see(short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the second of the two functions between colony_add_unit_2 and create_colony, which are already bound, paired in order; the arity agrees and the Mac body is 1.06x this one.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

int can_see(int colony, int nation)
{
    return g_colonies[colony].nation == (unsigned char)nation || g_game.reveal_all != 0 || g_colonies[colony].hist_pop[nation] != 0;
}
