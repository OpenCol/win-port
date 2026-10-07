/* europe_adjust_prices: candidate for 1048:42c8 (1716 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * THE EUROPEAN MARKET for the current nation (`g_cur_nation_rec`): target prices from what everyone has
 * sold, drift counters that move each price one step at a time, and the PRICEUP / PRICEDOWN news.
 * `reset` != 0 sets the processed goods straight to their targets and stops (the new-game call,
 * `europe_adjust_prices(1, -1)`). `only` limits the pass to one goods (-1 for all).
 *
 * VOLUME. Each goods' volume is a base word at SEG20:0x77ec plus every European nation's sales
 * of it (the long table at +0xfc of the nation record, negatives counted as 0). On nation 0's turn the
 * base decays by a 128th of the volume.
 *
 * TARGETS for two groups. Each group's goods share their total volume: `target = 3 * total / own
 * volume`, so the most-sold goods is the cheapest.
 *   - goods 9..12 (rum, cigars, cloth, coats): the counter moves by 100 x half the goods' up + down
 *     steps, toward the target. On `reset`, the price is the target clamped to the goods' lo..price_max;
 *   - goods 1..4 (sugar, tobacco, cotton, furs): furs count half their volume and get +1 before 1700
 *     and +1 again before 1600. The counter moves by half the steps, without the x 100.
 *
 * THE STEP, for every goods:
 *   - the counter gains the goods' own drift byte (doubled for nation 3 on odd turns);
 *   - at -100 x up or lower the price RISES by one, up to the cap;
 *   - at 100 x down or higher it FALLS by one, down to lo;
 *   - each move pays for itself out of the counter.
 * The cap is `hi`. For goods 14 and 15 (tools, muskets) on a computer nation's turn, the cap is
 * lowered by `2 * (4 - difficulty)` and by `(600 - turn) / 100`, and a price above it is pushed
 * down. A single-goods pass takes the drift back out, so a sale does not also age the market.
 * A computer nation's horses, tools and muskets are then held to `3 * (4 - difficulty) / 2 + 3`.
 * Finally the nation's goods-value byte (SEG30:0x10) becomes the price less one, at least 0.
 *
 * `moved` is set to 0 and tested but never set, so the fall test always runs, even after a rise.
 *
 * `cur = (long)g_cur_nation_rec->price[g]` needs its cast. Without it Borland widens byte to word to long in
 * two steps, where the shipped makes one `MOVSX EAX`.
 *
 * RENAMED 2026-09-26 from update_market_price: the Mac build's europe_adjust_prices(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller europe_new_turn; callee popm; callee popup_say), the runner-up gets 2, and the arity agrees (2). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "control.h"
#include "goods.h"
#include "nations.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern Nation g_nations[];         /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */

extern void popup_say(int slot, int word);
extern int popm(char *msg, int n);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern int europe_bid_price(int g);
extern void set_text_arg_i_1048_db6a(int slot, int v);

void europe_adjust_prices(int reset, int only)
{
    int step;
    int drift;
    int moved;
    int sign;
    signed char cap;
    signed char lim;
    long cur;
    long target;
    long total;
    long vol;
    int ta;
    int tb;
    long sold[16];
    register int good;
    register int n;

    for (good = 0; good < GOODS_COUNT; good++) {
        sold[good] = g_game.base_sales[good];
        for (n = 0; n < NATION_EUROPEAN_COUNT; n++)
            sold[good] += max_long_1048_dd87(0L, g_nations[n].sold[good]);
        if (reset == 0 && g_cur_nation == NATION_ENGLAND)
            g_game.base_sales[good] -= (int)(sold[good] >> 7);
    }
    total = sold[GOODS_RUM] + sold[GOODS_CIGARS] + sold[GOODS_CLOTH] + sold[GOODS_COATS];
    total = max_long_1048_dd87(1L, total);
    for (good = 9; good <= 12; good++) {
        if (only >= 0 && good != only)
            continue;
        sold[good] = max_long_1048_dd87(1L, sold[good]);
        target = total * 3 / sold[good];
        cur = (long)g_cur_nation_rec->price[good];
        sign = (int)cur - (int)target;
        sign = sign > 0 ? 1 : sign < 0 ? -1 : 0;
        step = (g_goods[good].down + g_goods[good].up) / 2;
        if (reset != 0) {
            target = max_long_1048_dd87(target, (long)g_goods[good].price_min);
            target = min_long_1048_ddb5(target, (long)g_goods[good].price_max);
            g_cur_nation_rec->price[good] = (signed char)target;
            continue;
        }
        g_cur_nation_rec->drift[good] += sign * step * 100;
    }
    if (reset != 0)
        return;
    total = (sold[GOODS_FURS] >> 1) + (sold[GOODS_SUGAR] + sold[GOODS_TOBACCO] + sold[GOODS_COTTON]);
    total = max_long_1048_dd87(1L, total);
    for (good = 1; good <= 4; good++) {
        if (only >= 0 && good != only)
            continue;
        vol = sold[good];
        if (good == 4)
            vol = vol >> 1;
        vol = max_long_1048_dd87(1L, vol);
        target = total * 3 / vol;
        if (good == 4) {
            if (g_game.year < 0x6a4)
                target++;
            if (g_game.year < 0x640)
                target++;
        }
        cur = (long)g_cur_nation_rec->price[good];
        sign = (int)cur - (int)target;
        sign = sign > 0 ? 1 : sign < 0 ? -1 : 0;
        step = (g_goods[good].down + g_goods[good].up) / 2;
        g_cur_nation_rec->drift[good] += sign * step;
    }
    for (good = 0; good < GOODS_COUNT; good++) {
        if (only >= 0 && good != only)
            continue;
        cap = g_goods[good].price_max;
        if (good >= 14 && (g_cur_nation >= NATION_EUROPEAN_COUNT || g_nation_text[g_cur_nation].control != CONTROL_HUMAN)) {
            cap -= (4 - g_game.difficulty) * 2;
            cap -= (600 - g_game.turn_counter) / 100;
            if (only < 0 && g_cur_nation_rec->price[good] > cap)
                g_cur_nation_rec->drift[good] += g_goods[good].down * 100;
        }
        moved = 0;
        drift = g_goods[good].drift;
        if (g_cur_nation == NATION_NETHERLANDS && (g_game.turn_counter & 1))
            drift = drift * 2;
        g_cur_nation_rec->drift[good] += drift;
        step = 0 - g_goods[good].up * 100;
        if (g_cur_nation_rec->drift[good] <= step) {
            g_cur_nation_rec->drift[good] -= step;
            if (g_cur_nation_rec->price[good] < cap) {
                g_cur_nation_rec->price[good]++;
                if (g_cur_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_nation].control == CONTROL_HUMAN) {
                    popup_say(0, g_goods_name_word[good]);
                    popup_say(1, g_nation_name_word[g_cur_nation]);
                    set_text_arg_i_1048_db6a(0, europe_bid_price(good));
                    popm("PRICEUP", 2);
                }
            }
        }
        if (moved == 0) {
            step = g_goods[good].down * 100;
            if (g_cur_nation_rec->drift[good] >= step) {
                g_cur_nation_rec->drift[good] -= step;
                if (g_goods[good].price_min < g_cur_nation_rec->price[good]) {
                    g_cur_nation_rec->price[good]--;
                    if (g_cur_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_nation].control == CONTROL_HUMAN) {
                        popup_say(0, g_goods_name_word[good]);
                        popup_say(1, g_nation_name_word[g_cur_nation]);
                        set_text_arg_i_1048_db6a(0, europe_bid_price(good));
                        popm("PRICEDOWN", 2);
                    }
                }
            }
        }
        if (only >= 0)
            g_cur_nation_rec->drift[good] -= drift;
        if (g_cur_nation >= NATION_EUROPEAN_COUNT || g_nation_text[g_cur_nation].control != CONTROL_HUMAN) {
            lim = 25;
            if (good == 15 || good == 14 || good == 8) {
                lim = ((4 - g_game.difficulty) * 3 >> 1) + 3;
                g_cur_nation_rec->price[good] = *min_char_ptr_1048_efbd(&lim, &g_cur_nation_rec->price[good]);
            }
        }
        g_goods_value[g_cur_nation][good] =
            *max_ptr_1048_efef((tb = 0, &tb), (ta = g_cur_nation_rec->price[good] - 1, &ta));
    }
}
