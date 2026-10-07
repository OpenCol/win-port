/* PORT: this file's own declarations of popi removed: the header declares it from the definition (W1 step 3). */
/* tribe_extortion: candidate for 1030:d2aa (1081 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A UNIT DEMANDS TRIBUTE FROM THE CURRENT NATIVE SETTLEMENT, one of the actions `1030:db76`
 * dispatches when a unit enters one. Four outcomes, one TEXT resource each.
 *
 * THE CONTEST, in the map region (plane-2 nibble) the unit stands in:
 *   - the THREAT is half the nation's word at SEG20:0xd9d6 plus its strength in that region
 *     (SEG20:0xdbb0). It grows by half for Spain and by half again with Hernan Cortes;
 *   - the RESISTANCE is the tribe's strength in the region (0xdc18) plus half its overall
 *     strength (0xdc00), doubled, plus half the tribe's alarm toward the nation.
 * The tribe is COWED when a roll 0..threat beats a roll 0..resistance, but never with no colony
 * of the nation enough to take the goods.
 *
 * THE ANSWER:
 *   - EXTORTLAUGH when not cowed and the resistance is at least the threat, or whenever the
 *     alarm is 75 or more;
 *   - EXTORTNO when not cowed and the alarm is 50 or more;
 *   - EXTORTPOOR when the settlement has already paid (flag 0x10) or is not cowed. The alarm then
 *     does not rise at all;
 *   - EXTORTSTUFF otherwise. The settlement is marked as having paid, and the most valuable of the
 *     tribe's goods (the 16-word table in data segment 25, after `indian_supply_and_demand`) goes into the
 *     nearest colony: `3 * value + 10` of it, at most 100, at most what the warehouse holds, and
 *     at least 10. The alarm rise is doubled.
 * The messages are shown only to a human nation, and the alarm rises (`piss_off`) by
 * difficulty + 1 for a human, or by 1.
 *
 * The nation is a `register` parameter (SI), and the threat a plain local declared FIRST so that
 * it is promoted into DI (rule 70). The difficulty-scaled rise is an if/else, not a `?:`.
 *
 * RENAMED 2026-09-26 from extort_from_tribe: the Mac build's tribe_extortion(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (callee find_close_colony; callee get_name; callee has_father; callee land_region_at; callee pissed), the runner-up gets 5, and the arity agrees (3).
 */

#include "colonization.h"

#include "control.h"
#include "goods.h"
#include "settle.h"
#include "nations.h"
#include "fathers.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern int land_region_at(int x, int y);
extern int pissed(int tribe, int i);
extern int has_father(int nation, int bit);

extern int find_close_colony(int x, int y, int nation, int land);
extern void popup_say(int slot, int word);
extern void sort_16x16(int n, int *payload, int *key);
extern void set_colony(int colony);
extern int get_warehouse(void);
extern void piss_off(int tribe, int nation, int delta, int unused);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern int get_name(int nation);
extern void indian_supply_and_demand(void);

void tribe_extortion(int u, register int nat, int named)
{
    int threat;
    int region;
    int x;
    int y;
    int resist;
    int alarm;
    int delta;
    int cowed;
    int col;
    int i;
    int goods;
    int amount;
    int cap;
    int idx[16];

    x = g_units[u].x;
    y = g_units[u].y;
    region = land_region_at(x, y);
    alarm = pissed(g_cur_tribe, nat);
    threat = (g_military_power[nat] >> 1) + g_nation_region_strength[nat][region];
    if (nat == NATION_SPAIN)
        threat += threat >> 1;
    if (has_father(nat, FATHER_HERNAN_CORTES))
        threat += threat >> 1;
    resist = g_tribe_region[g_cur_tribe][region] + (g_tribe_strength[g_cur_tribe] >> 1);
    resist = resist * 2 + (alarm >> 1);
    if (nat < NATION_EUROPEAN_COUNT && g_nation_text[nat].control == CONTROL_HUMAN)
        delta = g_game.difficulty + 1;
    else
        delta = 1;
    cowed = rand_range(0, threat) > rand_range(0, resist);
    col = find_close_colony(x, y, nat, region);
    if (col < 0)
        cowed = 0;
    if ((cowed == 0 && resist >= threat) || alarm >= 0x4b) {
        if (nat < NATION_EUROPEAN_COUNT && g_nation_text[nat].control == CONTROL_HUMAN) {
            popup_say(0, get_adjective(named));
            popi("EXTORTLAUGH", g_cur_tribe);
        }
    } else if (cowed == 0 && alarm >= 0x32) {
        if (nat < NATION_EUROPEAN_COUNT && g_nation_text[nat].control == CONTROL_HUMAN) {
            popup_say(0, g_difficulty_words[g_game.difficulty]);
            set_name_slot(1, g_nation_text[nat].leader);
            popup_say(2, get_name(named));
            popi("EXTORTNO", g_cur_tribe);
        }
    } else if ((g_cur_settlement_rec->flags & SETTLEMENT_EXTORTED) || cowed == 0) {
        if (nat < NATION_EUROPEAN_COUNT && g_nation_text[nat].control == CONTROL_HUMAN) {
            popup_say(0, g_difficulty_words[g_game.difficulty]);
            popup_say(1, get_name(named));
            popi("EXTORTPOOR", g_cur_tribe);
        }
        delta = 0;
    } else {
        delta = delta * 2;
        g_cur_settlement_rec->flags |= SETTLEMENT_EXTORTED;
        indian_supply_and_demand();
        for (i = 0; i < 16; i++)
            idx[i] = (unsigned char)i;
        sort_16x16(16, idx, g_native_sell);
        set_colony(col);
        cap = get_warehouse();
        goods = idx[15];
        amount = g_native_sell[GOODS_MUSKETS] * 3 + 10;
        amount = min_int_1030_eb20(amount, 100);
        amount = min_int_1030_eb20(amount, cap - g_cur_colony_rec->stock[goods]);
        amount = max_int_1030_ead6(amount, 10);
        popup_say(0, g_difficulty_words[g_game.difficulty]);
        popup_say(1, get_name(named));
        set_text_arg_i_1030_eb46(0, amount);
        popup_say(2, g_goods_name_word[goods]);
        set_name_slot(3, g_cur_colony_rec->name);
        if (nat < NATION_EUROPEAN_COUNT && g_nation_text[nat].control == CONTROL_HUMAN)
            popi("EXTORTSTUFF", g_cur_tribe);
        g_cur_colony_rec->stock[goods] += amount;
    }
    piss_off(g_cur_tribe, nat, delta, 0);
}
