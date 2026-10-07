/* build_another_colony: candidate for 1028:06cd (321 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `unit_build_another`'s first term, and the only one of the three that never looks at
 * a unit: everything it reads is per-NATION. Three early exits settle most calls -- zero once
 * there are 0x30 colonies ON THE MAP, all nations counted, and a flat 8 for a nation with no
 * colonies of its own or with its byte at DGROUP:0x4c06 clear.
 *
 * Otherwise it is a shortfall: `(g_nation_population[n] - colonies) / (4 - g_leader_traits[n][1])`, pulled halfway
 * toward `g_nation_cargo_capacity[n] / 2` from whichever side it lands on, and then reduced again when
 * `g_avg_colony_size[n]` is below `7 - 3 * g_leader_traits[n][1]`, by the amount it is short plus one, times the
 * colony count. Clamped at zero on the way out.
 *
 * `g_leader_traits` is read with a stride of FOUR and `g_avg_colony_size` with a stride of two, both indexed by
 * the same nation, so the first is a four-byte record per nation and the second a word.
 *
 * SEVEN LOCAL SLOTS, and two of them are the reason this took a second pass. The frame is
 * 14 bytes; five named values account for ten. The other two hold the divisor and a copy of
 * the colony count, both written immediately before the division that uses them -- so the
 * source computes them as separate statements rather than inline, and the compiler does not
 * invent them. A frame two bytes short is visible at the first instruction after the
 * prologue.
 *
 * `g_colonies_with_room` is reached DS-relative where the three DGROUP arrays above it need an explicit
 * segment load: one module, two declarations (rule 9).
 *
 * RENAMED 2026-09-26 from nation_score_base: the Mac build's build_another_colony(short), bound by call-graph agreement: 5 of its Mac call edges already bound land on this function (caller colony_plan; caller foreign_country; caller plan_move; caller unit_build_another), the runner-up gets 3; the arity agrees (1), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

/* same object: 4-byte `push cs; call ` */

int build_another_colony(register int n)
{
    int divisor;
    int have;
    int colonies;
    int t_a;
    int half;
    int need;
    int short_by;
    register int r;

    t_a = g_nation_population[n];
    colonies = g_nation_colony_count[n];
    if (g_game.colony_count >= 0x30)
        return 0;
    if (colonies < 1)
        return 8;
    if (g_colonies_with_room[n] == 0)
        return 8;
    divisor = 4 - g_leader_traits[n][1];
    have = colonies;
    r = (t_a - have) / divisor;
    half = g_nation_cargo_capacity[n] >> 1;
    if (r > half)
        r -= (r - half + 1) >> 1;
    else if (r < half)
        r += (half - r + 1) >> 1;
    need = -g_leader_traits[n][1] * 3 + 7;
    if (g_avg_colony_size[n] < need) {
        short_by = need - g_avg_colony_size[n];
        r -= g_nation_colony_count[n] * (short_by + 1);
    }
    r = max_int_1028_e350(0, r);
    return r;
}
