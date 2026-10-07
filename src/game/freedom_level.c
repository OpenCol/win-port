/* freedom_level: candidate for 1020:a5c0 (259 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A per-nation score scale: a base from the difficulty level (harder for the human
 * players among the first four, easier otherwise), times eight, raised by half at each of
 * 1600, 1650, 1700 and 1750, scaled by a per-nation byte plus one and halved when that
 * byte is zero; a flag bit replaces the result with 1500 * difficulty + 2000.
 *
 * RENAMED 2026-09-26 from score_from_difficulty_and_era: the Mac build's freedom_level(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between choose_freedom and add_liberty, which are already bound; the arity agrees and the Mac body is 0.90x this one.
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "records.h"

extern Nation g_nations[];   /* SEG20:0xc2f4 */
extern GameState g_game;   /* SEG20:0x7782 */

int freedom_level(int nation)
{
    int n = nation;
    int r;
    int v;

    v = (n < NATION_EUROPEAN_COUNT && g_nation_text[n].control == CONTROL_HUMAN) ? g_game.difficulty * 2 + 6 : 14 - g_game.difficulty;
    v = v << 3;
    if (g_game.year >= 1600) v += v >> 1;
    if (g_game.year >= 1650) v += v >> 1;
    if (g_game.year >= 1700) v += v >> 1;
    if (g_game.year >= 1750) v += v >> 1;
    r = (g_nations[n].fathers + 1) * v + 1;
    if (g_nations[n].fathers == 0)
        r = r >> 1;
    if (g_game.game_flags & 1)
        r = g_game.difficulty * 0x5dc + 0x7d0;
    return r;
}
