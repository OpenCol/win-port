/* piss_off: candidate for 1038:02a1 (679 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: piss_off
 *
 * Moves a tribe's alarm toward a European nation by `delta` and acts on the result. The value is
 * clamped to 0..100 in a table of 0x4e-byte rows -- the native records -- indexed by tribe then
 * nation. A rise is halved for NATION_FRANCE, whose advantage is exactly that ("colonies and
 * units cause alarm at only half the rate"), and halved again when bit 0x10 of the nation's
 * flags is set.
 *
 * THE FIRST ARGUMENT IS THE TRIBE AND THE SECOND THE NATION. This header had them the other way
 * round. The body settles it once the numbers have names: the first is used as
 * `tribe + NATION_EUROPEAN_COUNT`, the nation index of a native tribe, and the second is tested
 * `< NATION_EUROPEAN_COUNT` against the SEG20:0x7841 control table, and every caller passes a
 * tribe first (tribe_heresy, perform_orders_road,
 * set_field). Falling
 * below 0x4b clears the pair flags; reaching 100 with flag 0x40 set rolls against the
 * difficulty and may burn a mission. Otherwise, when the value crosses a multiple of five,
 * two display slots are refreshed, and on a NEGATIVE delta every settlement owned by the
 * tribe has its anger toward that nation capped -- to nothing when the standing fell by
 * more than a quarter-step, else to 0x20 or 0x60.
 *
 * The name is the Macintosh build's tribe_hostile(short, short, short, short): the arity
 * matches, the fourth argument is unused in both readings, and the 37 call sites are the
 * tribe and raid code -- negotiate_tribe, resolve_raid, indian_turn,
 * lost_city_exploration. It is a suggestion the call graph agrees with, not a proof.
 *
 * Two constructs here are in the shipped code and not artefacts of the reading: an
 * `if (delta < 0) {} else {}` that emits a test and a JMP SHORT 0, and an
 * `if (after > 0x4b && delta > 0) {}` with no body at all.
 *
 * RENAMED 2026-09-27 from tribe_hostile: Call graph: Mac piss_off(short x4) has 18 callers, and the 18 callers of 02a1 are exactly the same names: combat_fight, give_me_liberty, indian_turn, indian_village, lost_city_exploration, move_unit, negotiate_europe, negotiate_tribe, perform_orders_clear, perform_orders_road, resolve_raid, set_field, tribe_extortion, tribe_heresy, tribe_incite, tribe_live, tribe_mission and tribe_trade. The Mac sequence has_father, treaty_off x2, get_treaty, random, tribe_burn_missions, get_adjective x2, popup_say x2 matches 02a1's callees has_father, treaty_off x2, get_treaty, rand_range, tribe_burn_missions, get_name x2 (the Windows get_adjective, see 1030:8033) and popup_say x2. Arity: 4 shorts against tribe_hostile(int tribe, int nation, int delta, int unused). Size is 644 Mac against 679 Windows. Layout: Mac CODE runs tribe_burn_missions, piss_off, find_village, find_close_village, village_anger, and Windows 1038 runs tribe_burn_missions (0162), mission_burned (022f), 02a1, find_village (0548), find_close_village (05b8), village_anger (06b6). The body raises a tribe's alarm toward a nation, which is what piss_off means. The current name was the reconstructor's descriptive word and happens to be a Mac identifier, but the Mac tribe_hostile is 1030:c850. This rename must come first in its chain.
 */

#include "colonization.h"

#include "fathers.h"
#include "control.h"
#include "nations.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int pissed(int tribe, int b);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int piss_factor(int tension);
extern int has_father(int who, int bit);
extern int treaty_off(int nation, int other, int mask);
extern int get_treaty(int nation_a, int nation_b);

extern void tribe_burn_missions(int tribe, int nation);

extern void popup_say(int slot, int v);

void piss_off(int tribe, int nation, int delta, int unused)
{
    register int nat = nation;
    int after;
    int before;
    int q_before;
    int q_after;
    int owner;
    int i;

    before = pissed(tribe, nat);
    before = minimax(before, 0, 100);
    q_before = piss_factor(before);
    if (nat == NATION_FRANCE && delta > 0)
        delta = delta >> 1;
    if (has_father(nat, FATHER_POCAHONTAS) && delta > 0)
        delta = delta >> 1;
    after = g_tribes[tribe].anger[nat] + delta;
    after = minimax(after, 0, 100);
    g_tribes[tribe].anger[nat] = after;
    q_after = piss_factor(after);
    q_before = q_before >> 1;
    q_after = q_after >> 1;
    if (delta < 0) {
        treaty_off(tribe + NATION_EUROPEAN_COUNT, nat, 4);
        if (after < 0x4b)
            treaty_off(tribe + NATION_EUROPEAN_COUNT, nat, 2);
    }
    if (after > 0x4b && delta > 0) {
    }
    if (after >= 100 && (get_treaty(tribe + NATION_EUROPEAN_COUNT, nat) & 0x40)) {
        if (rand_range(0, 10) <= ((nat < NATION_EUROPEAN_COUNT && g_nation_text[nat].control == CONTROL_HUMAN) ? g_game.difficulty : 1) + 1)
            tribe_burn_missions(tribe, nat);
        goto done;
    }
    after = min_int_1038_8680(0x63, after);
    if (before / 5 != after / 5) {
        popup_say(0, get_adjective(nat));
        popup_say(1, get_adjective(tribe + NATION_EUROPEAN_COUNT));
        if (delta < 0) {
        } else {
        }
        if (delta < 0) {
            owner = tribe + NATION_EUROPEAN_COUNT;
            for (i = 0; i < g_game.settlement_count; i++) {
                if (g_settlements[i].owner != (unsigned char)owner)
                    continue;
                if (q_before - q_after > 1)
                    g_settlements[i].anger[nat] = 0;
                else if (q_after == 0)
                    g_settlements[i].anger[nat] =
                        min_int_1038_8680(g_settlements[i].anger[nat], 0x20);
                else
                    g_settlements[i].anger[nat] =
                        min_int_1038_8680(g_settlements[i].anger[nat], 0x60);
            }
        }
    }
done:
    ;
}
