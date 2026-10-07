/* village_bribe: candidate for 1038:0c18 (455 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 455, not the census 446. The trailing 9 bytes are the dead second epilogue.
 *
 * WHAT A TRIBE ASKS FOR A SQUARE OF ITS LAND. `orders_clear` calls it with the settlement
 * that claims the square and shows the answer in the `INDIANFOREST` popup.
 *
 * The base is two of the tribe record's bytes (+2 and +5) less the square's octile distance
 * from the settlement, plus 6 and twice the difficulty for a human nation or 12 less the
 * difficulty for anyone else; then less half of what the nation's figure at DGROUP:0x4b54 falls
 * short of 10, never less than nothing. A square with scenery on it (`resource_at` not -1)
 * counts double. The result, at least 1, is multiplied by 65 for a human and 50 otherwise; a
 * human's price is multiplied again by one plus the tribe's word for the player nation (+0x46
 * of its record, `pissed`) bucketed into quarters of 100 -- so 1x to 4x -- a settlement
 * with bit 2 of its flags byte (+3) asks half as much again, and the whole is halved at the end.
 *
 * NATION FLAG 2 MAKES IT FREE. That is Peter Minuit -- the same flag clears every native claim
 * in `colony_indian_table`, which is the other half of his effect in the game.
 *
 * `max_ptr_1038_8d92((zero = 0, &zero), (pop_shortfall = ..., &pop_shortfall))` is rule 61's comma form: each
 * store precedes its `LEA`, and the SS push comes after the `LEA`. `price = price - ...`,
 * `price = price * 2` and `price = price >> 1` load, operate and store where the compound forms
 * would work on SI in place; `price += price >> 1` is the one the shipped does in place
 * (rule 60). `price` is a `register` local and takes SI, the `register` parameter `nation` DI
 * (rule 53). Four helpers come through the 4-byte `PUSH CS` form and are stubs; `max_ptr`
 * through the 5-byte one and is extern (rule 47).
 *
 * RENAMED 2026-09-26 from native_land_price: the Mac build's village_bribe(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller orders_clear; caller orders_road; caller set_field; caller try_to_set_job; callee has_father), the runner-up gets 4, and the arity agrees (4).
 */

#include "colonization.h"

#include "nations.h"
#include "fathers.h"
#include "control.h"
#include "settle.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */
extern int map_dist(int x1, int y1, int x2, int y2);

extern int resource_at(int x, int y);
extern int has_father(int nation, int bit);

/* same object: 4-byte `push cs; call ` */
extern void set_village(int s);

extern int pissed(int tribe, int i);
extern int piss_factor(int tension);

int village_bribe(int settlement, register int nation, int x, int y)
{
    int dist;
    int mult;
    int pop_shortfall;
    int zero;
    register int price;

    set_village(settlement);
    dist = map_dist(x, y, g_cur_settlement_rec->x, g_cur_settlement_rec->y);
    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
        price = g_cur_tribe_rec->level + g_game.difficulty * 2 + 6 - dist + g_cur_tribe_rec->count;
        mult = 0x41;
    } else {
        price = g_cur_tribe_rec->level + 0xc - dist - g_game.difficulty + g_cur_tribe_rec->count;
        mult = 0x32;
    }
    price = price - *max_ptr_1038_8d92((zero = 0, &zero),
                                       (pop_shortfall = (10 - g_nation_population[nation]) >> 1, &pop_shortfall));
    if (resource_at(x, y) != -1)
        price = price * 2;
    price = max_int_1038_86a6(1, price);
    price = price * mult;
    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN)
        price = (piss_factor(pissed(g_cur_tribe, g_game.turn_nation)) + 1) * price;
    if (g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL)
        price += price >> 1;
    if (has_father(nation, FATHER_PETER_MINUIT) != 0)
        price = 0;
    price = price >> 1;
    return price;
}
