/* assess_upkeep: candidate for 1020:82c7 (72 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Clears bit 4 of the current nation's first byte, having read the bit and the
 * word at SEG20:0x7796 into locals that nothing then uses -- what this
 * returned was removed and the reads were left behind.
 *
 * RENAMED 2026-09-26 from clear_nation_flag_10: the Mac build's assess_upkeep(void), bound by call-sequence alignment (1.0 aligned gaps in new_turn_bookkeeping leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.81x this one. The Windows body keeps only the clear of one nation flag; the reads beside it are what is left of the upkeep assessment.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

void assess_upkeep(void)
{
    int nation;
    int b = 0;
    int c;

    nation = g_game.turn_nation;
    c = g_cur_nation_rec->flags & 0x10;
    g_cur_nation_rec->flags &= 0xef;
}
