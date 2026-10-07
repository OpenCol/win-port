/* Century: candidate for 1020:9884 (52 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * How many of two thresholds a global counter has reached: 0, 1 or 2.
 *
 * RENAMED 2026-09-26 from threshold_rank: the Mac build's Century(void), bound by call-sequence alignment (the lone unbound callee of its gap in avail_of_father); the arity agrees and the Mac body is 0.92x this one. Its body counts how many of two year thresholds have passed -- the century.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
int Century(void)
{
    int n = 0;
    if (g_game.year >= 0x640) n++;
    if (g_game.year >= 0x6a4) n++;
    return n;
}
