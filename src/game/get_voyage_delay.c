/* get_voyage_delay: candidate for 1030:2986 (280 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Returns the initial value of the per-unit counter at +0x16 that `send_unit_to_europe`
 * writes into every unit of the stack. It is 1, unless a 1..100 roll comes up 90 or better AND
 * the nation's byte at DGROUP:0x4b60 is 3 or more AND nation flag 5 is clear -- then 2. So the
 * long value is the rare one, at roughly one arrival in ten and only for nations that qualify.
 *
 * SIXTEEN OF THESE BYTES COMPUTE A NUMBER AND THROW IT AWAY. When the second argument is 2 or
 * less the function draws another random number, starts from 3, and adjusts: minus one for
 * nation flag 5, minus two for unit type 0x10, minus one for 0xf, plus one for 0xd, minus two
 * for a non-European nation or a nation record whose first byte is set, then clamped at zero
 * with `max_int_1030_ead6`. The result goes into SI and SI is never read again: the function
 * returns the 1-or-2 from the first block.
 *
 * That block is not dead in the sense the optimiser would care about -- `rand_range` advances
 * the generator, so calling it CHANGES every later roll in the turn -- but its value is gone.
 * It reads like an older, graded formula for the same counter, left wired up after a simpler
 * rule replaced it.
 *
 * THE THIRD PARAMETER IS UNUSED. Both call sites push three words; the body reads only the
 * first two. Nothing in these bytes can show the third (§119).
 *
 * The first test is the NEGATIVE form -- `if (roll < 90 || byte < 3 || flag) r = 1; else r = 2;`
 * -- and the positive `&&` spelling swaps the two stores and the sense of the last jump.
 *
 * The nation is masked in eight bits, which is rule 58: `(unsigned char)(flags & 0xf)`.
 *
 * RENAMED 2026-09-26 from arrive_counter_start: the Mac build's get_voyage_delay(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller new_turn_bookkeeping; caller set_course_for_new_world; callee has_father), the runner-up gets 1, and the arity agrees (3).
 */

#include "colonization.h"

#include "nations.h"
#include "fathers.h"
#include "control.h"
#include "units.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern void random_activate(int seed);

extern int has_father(int nation, int bit);

/* same object: 4-byte `push cs; call ` */

int get_voyage_delay(int u, int tile_x, int unused1)
{
    int roll;
    int r;
    register int score;
    register int nation;

    random_activate(g_random_seed);
    roll = rand_range(1, 100);
    nation = NATION(g_units[u]);
    if (roll < 0x5a || g_nation_ship_count[nation] < 3 || has_father(nation, FATHER_FERDINAND_MAGELLAN) != 0)
        r = 1;
    else
        r = 2;
    score = 0;
    if (tile_x <= 2) {
        score = rand_range(0, 1) + 3;
        if (has_father(nation, FATHER_FERDINAND_MAGELLAN) != 0)
            score--;
        if (g_units[u].type == UNIT_PRIVATEER)
            score -= 2;
        if (g_units[u].type == UNIT_GALLEON)
            score--;
        if (g_units[u].type == UNIT_CARAVEL)
            score++;
        if (nation >= NATION_EUROPEAN_COUNT || g_nation_text[nation].control != CONTROL_HUMAN)
            score -= 2;
        score = max_int_1030_ead6(score, 0);
    }
    return r;
}
