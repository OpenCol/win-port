/* PORT: this file's own declarations of popi removed: the header declares it from the definition (W1 step 3). */
/* tribe_hostile: candidate for 1030:c850 (197 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * How a tribe reacts to a wagon train on its land. A roll of 0..500 is compared against a
 * per-tribe threshold from the 78-byte table, and the three outcomes are three text
 * resources, all read out of the shipped image:
 *
 *   roll <= threshold        KILLWAGONS    -- the wagon is destroyed and 1 is returned
 *   roll <= threshold * 2    MADATWAGONS   -- a complaint
 *   otherwise                GRUDGEWAGONS  -- a complaint, then the trade goes ahead (`tribe_trade`)
 *
 * Returning 1 only in the first case is the caller's signal that the unit index it passed
 * is gone: `delete_unit` compacts the array, so anything holding an index above it is
 * stale. The same contract as `process_off_board_ships` and `kill_stack`.
 *
 * THE NAME IS NOT THE ONE THE SYMBOL MAP HAD. `tools/re_symbols.py` fell back to
 * `txt_GRUDGEWAGONS_1030_c850` because the record carried no curated name and the function
 * references a TEXT identifier -- and it picked `ids[0]`, the first of the three. That is a
 * placeholder by construction, and naming a function after the *third* of its three
 * outcomes gets it backwards: GRUDGEWAGONS is the case where the wagon survives and the trade goes ahead. The set
 * {KILLWAGONS, MADATWAGONS, GRUDGEWAGONS} shares the suffix WAGONS rather than a prefix,
 * which is why `re_textnames.py`'s themed-set rule -- it looks for a shared >=4-character
 * PREFIX -- did not catch it either.
 *
 * `g_cur_tribe` is a word this function only passes on; what it selects in the 78-byte
 * table is not established here.
 *
 * RENAMED 2026-09-27 from tribe_wagon_reaction: String witness: GRUDGEWAGONS, KILLWAGONS and MADATWAGONS, all df 1/1, are in Mac tribe_hostile(short x4), with a score of 49.71 against a runner-up of 0. Call graph: the Mac function's only caller is tribe_village, and c850's only caller is tribe_village (1030:db76). The Mac sequence pissed, random, get_adjective, popup_say, popm, delete_unit, popi x2, tribe_trade matches c850's callees pissed, rand_range, get_name (=Mac get_adjective), popup_say, popm, delete_unit, popi x2 and tribe_trade. Arity: 4 shorts against tribe_wagon_reaction(int unit, int settlement, int nation, int tribe). Size is 202 Mac against 197 Windows. The old name is descriptive. Apply this only after 1038:02a1 has moved to piss_off.
 */

#include "colonization.h"

extern int pissed(int a, int tribe);

extern void popup_say(int slot, int v);
extern int popm(char *msg, int n);
extern void delete_unit(int u);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */

extern void tribe_trade(int unit, int settlement, int nation, int tribe_nation);

int tribe_hostile(int unit, int settlement, int nation, int tribe)
{
    int result;
    int threshold;
    int roll;

    result = 0;
    threshold = pissed(g_cur_tribe, nation);
    roll = rand_range(0, 0x1f4);
    popup_say(0, get_adjective(tribe));
    if (roll <= threshold) {
        popm("KILLWAGONS", 3);
        delete_unit(unit);
        result = 1;
    } else if (threshold * 2 >= roll) {
        popi("MADATWAGONS", g_cur_tribe);
    } else {
        popi("GRUDGEWAGONS", g_cur_tribe);
        tribe_trade(unit, settlement, nation, tribe);
    }
    return result;
}
