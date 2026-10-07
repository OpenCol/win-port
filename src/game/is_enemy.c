/* PORT: g_leader_traits is LeaderFace[] (bytes a, b, c): [n][0..2] read as .a/.b/.c, signed as
   the corpus's signed char view read them (W1). */
/* is_enemy: candidate for 1030:4c87 (757 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 757, not the census 748. The trailing 9 bytes are the dead last epilogue.
 *
 * Whether European nation `a` would break with nation `b`, the test `negotiate_ai`
 * makes in both directions before signing or cancelling a treaty. The answer is 0 (keep the peace)
 * when:
 *
 *   - the nation at SEG20:0x779a is the one recorded at SEG25:0x6, or fewer than 40 turns have passed
 *   - both nations' bytes at DGROUP:0x4b50 are under 8
 *   - either nation has bit 2 of its record's first byte set
 *   - `a` or `b` has pair flags 0x20 without 0x40 towards the nation at 0x779a, and that nation
 *     outranks it on the word table at SEG20:0xd9d6 or the byte table at DGROUP:0x4b54 -- except
 *     that the check for `b` compares the byte table against `a`'s entry, not `b`'s, a copy of the
 *     `a` block with one index left unchanged
 *   - in any of regions 1..14 where `a` has an entry in the table at SEG20:0xdab0, half of `a`'s
 *     entries in the tables at 0xda70 and 0xdb70 fall short of `b`'s entry in 0xdb70
 *
 * Otherwise `a` counts the other powers it holds 0x20-without-0x40 relations with, adds one unless
 * it holds that with `b`, and subtracts the signed byte at SEG20:0x7c18 + a*4. Summing over the
 * regions where `a` has 0xdb70 and `b` has 0xda70, it compares 4 * a's 0xdb70 total against b's
 * 0xda70 + half 0xdb70 total (plus 1): at least that count + 4, or no 0xdab0 entries at all, means 1.
 *
 * RENAMED 2026-09-26 from nation_wants_war: the Mac build's is_enemy(short, short), bound by call-graph agreement: 1 of its Mac call edges already bound land on this function (caller negotiate_ai), the runner-up gets 0; and by call-sequence alignment: in negotiate_ai the calls either side align and this is the one unbound callee left in that gap (1.0 votes, next 0.0); the arity agrees (2), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern int g_rank_ids[4];                   /* SEG25:0x0 -- 0x6 is [3] */

extern Nation g_nations[];             /* SEG20:0xc2f4 */

/* 5-byte NOP form, so extern (rule 47) */
extern int get_treaty(int nation_a, int nation_b);

int is_enemy(register int nation, int other)
{
    int me;
    int got;
    int want;
    int any;
    int count;
    register int i;

    if ((unsigned char)g_game.player_nation == g_rank_ids[3])
        return 0;
    if (g_game.turn_counter < 40)
        return 0;
    if (g_pop_by_nation[nation] < 8 && g_pop_by_nation[other] < 8)
        return 0;
    me = g_game.player_nation;
    if (g_nations[nation].flags & 4)
        return 0;
    if (g_nations[other].flags & 4)
        return 0;
    if ((get_treaty(nation, me) & 0x60) == 0x20
        && (g_military_power[me] > g_military_power[nation] || g_nation_population[me] > g_nation_population[nation]))
        return 0;
    if ((get_treaty(other, me) & 0x60) == 0x20
        && (g_military_power[me] > g_military_power[other] || g_nation_population[me] > g_nation_population[nation]))
        return 0;
    count = 0;
    for (i = 0; i < 4; i++) {
        if (i == nation)
            continue;
        if (i == other)
            continue;
        if ((get_treaty(nation, i) & 0x60) == 0x20)
            count++;
    }
    if ((get_treaty(nation, other) & 0x60) != 0x20)
        count++;
    count -= ((signed char)g_leader_traits[nation].a);
    got = any = 0;
    want = 1;
    for (i = 1; i < 15; i++) {
        if (g_region_colony_count[nation][i] != 0
            && (unsigned char)(g_region_unit_count[nation][i] >> 1) + (unsigned char)(g_region_attack[nation][i] >> 1) < g_region_attack[other][i])
            return 0;
        if (g_region_attack[nation][i] != 0 && g_region_unit_count[other][i] != 0) {
            got += g_region_attack[nation][i];
            want += g_region_unit_count[other][i] + (g_region_attack[other][i] >> 1);
            any += g_region_colony_count[nation][i];
        }
    }
    if (got * 4 / want >= count + 4 || any == 0)
        return 1;
    return 0;
}
