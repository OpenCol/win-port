/* tribe_trade: candidate for 1030:b61e (4658 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: tribe_trade
 * name-source: sibling-build
 *
 * TRADING WITH A NATIVE VILLAGE, both halves of it: the wagon or ship sells one cargo and then buys
 * one back, with a haggle in the middle of each. The village and tribe are whichever
 * `set_village` and `set_tribe` left selected; a nation below zero means
 * only the price tables are wanted, and the function returns after building them.
 *
 * WHAT THE VILLAGE WILL PAY comes from `indian_supply_and_demand`, which fills SEG25:0x10 (what
 * it pays) and SEG25:0x30 (what it asks). This function then edits the tables before showing them:
 * food is never bought, and the three goods the village asks MOST for have their buying price zeroed
 * so it cannot both want a thing badly and pay for it -- with a zero rewritten to 12, so the "bring
 * us" list never says "food".
 *
 * THE SELL. With more than one cargo aboard, a human picks from a TRADEWHICH menu of "<amount>
 * <goods>" lines and a computer picks at random. Three things stop the sale before a price is named:
 * the goods the village last bought or sold, or anything it pays nothing for, get BADCARGO with the
 * three goods it would rather have; and the goods it refused last time gets BADHAGGLE1.
 *
 * THE PRICE is the village's buying price times a margin -- `(4 - difficulty + bonus + roll -
 * discount) * 2` -- scaled by the amount, halved, and floored at 1. The bonus starts at 6 and moves
 * with the goods: one more above goods 8, a random 0..7 LESS for goods 13, and for muskets and horses
 * it is what the tribe still lacks (12 or 10 less what it has). The discount is the tribe's tension
 * through `piss_factor`, doubled, dropped entirely for muskets and horses and halved
 * again once the village pays 20 or more. The same figure sets the adjective (`g_value_word[tier]`)
 * and the village's patience, `mood`.
 *
 * THE HAGGLE is the TRADE0/TRADE1 dialog. Asking more works while `mood` is positive and a roll of
 * `rand_range(1, mood * 8)` beats the difficulty: the price goes up by a slice of the amount and the
 * dialog comes back as TRADE1. When it does not work the village refuses this cargo for good
 * (BADHAGGLE0) and the whole trade ends. Accepting pays the nation, moves the goods into the tribe's
 * stock, calms the village and -- for muskets and horses -- arms it: 25 units adds one to the tribe's
 * armed braves or mounted braves, 50 adds two, and a quarter of any horses sold joins the herd.
 * Giving the cargo away (answer 3, only on the first round) calms it twice as much.
 *
 * THE BUY works the same way in reverse: BUYWHICH offers the three goods the village asks most for,
 * skipping food, muskets, tools (14) and trade goods (13); a computer picks the one worth most to its own nation.
 * The price starts at 200, moves with the tribe's level and the goods' value to the buyer, takes a
 * random slice of itself, and is floored at 50. Haggling down subtracts `max(1, price / 4)` and is
 * refused once the price is at 10 or the difficulty beats the roll -- which sets the village's
 * refusal marker to -2 and shows BADHAGGLE2, the state that gives BADHAGGLE3 on the next visit.
 * A ship carries a quarter of what a wagon does.
 *
 * TWO SHIPPED BUGS. Both places that remember what the village last bought test `unit != 0xf && unit
 * != 8` -- the UNIT index against goods numbers -- where they plainly mean the goods just sold, so
 * unit 15 and unit 8 are the only two units in the game that clear the marker instead of setting it.
 * And the second parameter is never read.
 *
 * WHAT THE BYTES SETTLED: the human flag is a TERNARY over a boolean, `(nation < 0) ? 0 : (nation < 4
 * && ...)`, because the shipped emits its own `XOR AX,AX` for the first arm rather than sharing the
 * one at the end. Every comparison of the village's three signed-char markers against a constant needs
 * an explicit `(int)`, or Borland folds it into one byte. `(buy + 1) << 2` must be a SHIFT -- written
 * `* 4` the compiler distributes it to `buy * 4 + 4`. The last `popup_say` takes a
 * ternary as its argument, not two calls. And the buy half is a structured `if (ok != 0) { ... }`,
 * because the shipped test is one long conditional and not rule 12's short-hop-over-a-jump.
 *
 * RENAMED 2026-09-26 from native_village_trade: the Mac build's tribe_trade(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 10 of its Mac call edges already bound land on this function (callee get_cargo; callee get_cargo_amount; callee get_treaty; callee item; callee pissed), the runner-up gets 6, and the arity agrees (4). The Mac foreign_cargo also points here (8 vs 4) but lacks tribe_trade's call to pissed, the tribe-anger query a native trade makes.
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "goods.h"
#include "units.h"
#include "misc.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern Nation g_nations[];               /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_mood(int mood);
extern void random_activate(int seed);
extern void sort_16x16(int n, int *payload, int *key);
extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);

extern void space_string(char *buf);

extern void Popup_item(Popup *popup, char *s, int id);
extern char *strings(int n);
extern void popup_say(int slot, int word);
extern int piss_factor(int tension);

extern int unload_cargo(int u, int slot);
extern void piss_off(int tribe, int nation, int delta, int unused);
extern int stow_cargo(int u, int goods, int amount);

extern int pissed(int tribe, int i);
extern int popi(char *name, int tribe);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void indian_supply_and_demand(void);

extern "C" unsigned char get_treaty(int nation_a, int nation_b);

void tribe_trade(int unit, int settlement, int nation, int tribe_nation)
{
    int goods = -1;
    int roll;
    int margin;
    int tier;
    int again;
    int discount;
    int mood;
    int ok = 1;
    int limit;
    int round;
    int answer;
    int i;
    int sel;
    int bonus;
    int step;
    int amount;
    int bump;
    int human;
    int tension;
    int best;
    int good;
    int slot;
    int half;
    int tmp;
    int n;
    int pick[3];
    int t1;
    int t2;
    int t3;
    int t4;
    int order[16];
    int buy_order[16];
    char msg[0x28];
    char buf[0x50];
    Popup p(0x800);
    register int price;

    human = (nation < 0) ? 0 : (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN);
    if (human != 0 && rand_range(0, 3) == 0) {
        set_mood(5);
        if (g_cur_tribe == 0)
            set_mood(7);
        if (g_cur_tribe == 1)
            set_mood(6);
    }
    if (g_units[unit].type < UNIT_SHIP_FIRST || g_units[unit].type > UNIT_SHIP_LAST)
        g_units[unit].hold.w.trade_loaded = 0;
    for (i = 0; i < 16; i++)
        order[i] = buy_order[i] = (unsigned char)i;
    random_activate(g_random_seed);
    indian_supply_and_demand();
    tmp = g_native_sell[GOODS_FOOD];
    g_native_sell[GOODS_FOOD] = 0;
    if (g_native_sell[GOODS_TRADE_GOODS] < tmp)
        g_native_buy[GOODS_FOOD] = 0;
    sort_16x16(0x10, order, g_native_sell);
    for (i = 1; i <= 3; i++) {
        g_native_buy[order[0x10 - i]] = 0;
        if (order[0x10 - i] == 0)
            order[0x10 - i] = 0xc;
    }
    if (nation < 0)
        return;
    if (g_units[unit].cargo_count != 0) {
        slot = 0;
        if (g_units[unit].cargo_count > 1) {
            if (human == 0)
                slot = rand_range(0, g_units[unit].cargo_count - 1);
            else {
                show_plain_dialog_1030_ebb7(&p, "TRADEWHICH", 0);
                for (i = 0; i < g_units[unit].cargo_count; i++) {
                    goods = get_cargo(unit, i);
                    amount = get_cargo_amount(unit, i);
                    LTOA1(amount, buf, 10);
                    space_string(buf);
                    say_string(buf, g_goods_name_word[goods]);
                    Popup_item(&p, buf, i + 1);
                }
                Popup_item(&p, strings(g_misc_words[MISC_NOTHING]), 0x63);
                answer = popup_run_thunk_1030_ebe7(&p) - 1;
                if (answer == 0 || answer == 0x63)
                    goto done;
                slot = answer - 1;
            }
        }
        goods = get_cargo(unit, slot);
        amount = get_cargo_amount(unit, slot);
    }
    tension = pissed(g_cur_tribe, nation);
    if (goods >= 0) {
        if (human != 0) {
            if (g_cur_settlement_rec->sold == goods ||
                g_cur_settlement_rec->bought == goods ||
                g_native_buy[goods] == 0) {
                popup_say(0, g_goods_name_word[goods]);
                if ((int)g_cur_settlement_rec->sold >= 0)
                    g_native_buy[g_cur_settlement_rec->sold] = 0;
                if ((int)g_cur_settlement_rec->bought >= 0)
                    g_native_buy[g_cur_settlement_rec->bought] = 0;
                sort_16x16(0x10, buy_order, g_native_buy);
                popup_say(1, g_goods_name_word[buy_order[15]]);
                popup_say(2, g_goods_name_word[buy_order[14]]);
                popup_say(3, g_goods_name_word[buy_order[13]]);
                popi("BADCARGO", g_cur_tribe);
                goto done;
            }
            if (g_cur_settlement_rec->refused == goods) {
                popup_say(0, g_goods_name_word[goods]);
                popi("BADHAGGLE1", g_cur_tribe);
                goto done;
            }
        }
        roll = rand_range(1, 5);
        bonus = 6;
        if (goods > 8)
            bonus++;
        if (goods == 0xd)
            bonus -= rand_range(0, 7);
        if (goods == 0xf)
            bonus += 0xc - g_cur_tribe_rec->muskets;
        if (goods == 8)
            bonus += 0xa - g_cur_tribe_rec->horse_herds;
        if (goods == 0xe)
            bonus++;
        discount = piss_factor(tension) * 2;
        if (goods == 0xf || goods == 8)
            discount = 0;
        if (g_native_buy[goods] >= 0x14)
            discount = discount >> 1;
        margin = (4 - g_game.difficulty + bonus + roll - discount) * 2;
        price = g_native_buy[goods] * margin;
        price = max_int_1030_ead6(0, price);
        price += roll * 5;
        price = (int)((long)price * amount / 100L);
        price = price / 2;
        price = max_int_1030_ead6(1, price);
        tier = (g_native_buy[goods] + 4 - discount) / 10;
        tier = min_int_1030_eb20(tier, 3);
        mood = (g_native_buy[goods] + 4 - discount) >> 2;
        mood += rand_range(0, 1);
        mood -= 4;
        half = amount;
        limit = ((g_native_buy[goods] + 1) << 2) + price;
        popup_say(0, g_value_word[tier]);
        popup_say(1, g_goods_name_word[goods]);
        round = 0;
        STRCPY(msg, "TRADE0");
        do {
            again = 0;
            g_text_args[0] = price;
            g_text_args[1] = limit;
            msg[5] = (char)(round + 0x30);
            if (human != 0)
                answer = popi(msg, g_cur_tribe);
            else {
                answer = 1;
                if (tension >= 0x32)
                    answer = 3;
            }
            switch (answer) {
            case 1:
                unload_cargo(unit, slot);
                g_nations[nation].gold += price;
                g_cur_tribe_rec->goods_drift[goods] += g_cargo_amount;
                g_cur_settlement_rec->refused = -1;
                if (mood > 0) {
                    piss_off(g_cur_tribe, nation, -(mood * 2), 0);
                    g_cur_settlement_rec->anger[nation] -= g_cargo_amount;
                    g_cur_settlement_rec->anger[nation] =
                        max_int_1030_ead6(0, g_cur_settlement_rec->anger[nation]);
                    if (g_cargo_amount == 100)
                        g_cur_settlement_rec->anger[nation] = 0;
                }
                if (unit != 0xf && unit != 8)
                    g_cur_settlement_rec->sold = (char)goods;
                else
                    g_cur_settlement_rec->sold = -1;
                if (goods == 0xf) {
                    if (amount >= 0x19)
                        g_cur_tribe_rec->muskets++;
                    if (amount >= 0x32)
                        g_cur_tribe_rec->muskets++;
                }
                if (goods == 8) {
                    g_cur_tribe_rec->horses += amount >> 2;
                    if (amount >= 0x19)
                        g_cur_tribe_rec->horse_herds++;
                    if (amount >= 0x32)
                        g_cur_tribe_rec->horse_herds++;
                }
                break;
            case 2:
                half = half >> 1;
                if (mood > 0 && rand_range(1, mood * 8) > g_game.difficulty) {
                    mood--;
                    bump = rand_range((g_native_buy[goods] >> 1) + 1,
                                      g_native_buy[goods] * 2 + 1);
                    bump = bump * amount / 100;
                    bump = max_int_1030_ead6(1, bump);
                    price += bump;
                    if (price >= limit)
                        limit = price + 10;
                    round = 1;
                    again = 1;
                } else {
                    g_cur_settlement_rec->refused = (char)goods;
                    piss_off(g_cur_tribe, nation, (discount >> 1) + 1, 0);
                    if ((get_treaty(nation, tribe_nation) & 0x40) != 0)
                        popi("BADHAGGLE0", g_cur_tribe);
                    ok = 0;
                }
                break;
            case 3:
                if (round == 0) {
                    unload_cargo(unit, slot);
                    g_cur_settlement_rec->refused = -1;
                    if (unit != 0xf && unit != 8)
                        g_cur_settlement_rec->sold = (char)goods;
                    else
                        g_cur_settlement_rec->sold = -1;
                    if (mood >= 0) {
                        mood++;
                        piss_off(g_cur_tribe, nation, -(mood * 4), 0);
                        g_cur_settlement_rec->anger[nation] -= g_cargo_amount * 2;
                        g_cur_settlement_rec->anger[nation] =
                            max_int_1030_ead6(0, g_cur_settlement_rec->anger[nation]);
                        if (g_cargo_amount == 100)
                            g_cur_settlement_rec->anger[nation] = 0;
                    }
                    if (goods == 0xf)
                        g_cur_tribe_rec->muskets++;
                    if (goods == 8) {
                        g_cur_tribe_rec->horses += g_cargo_amount >> 2;
                        g_cur_tribe_rec->horse_herds++;
                    }
                } else
                    ok = 0;
                break;
            default:
                ok = 0;
                break;
            }
        } while (again != 0);
    }
    if (g_unit_types[g_units[unit].type].holds - g_units[unit].cargo_count < 1)
        ok = 0;
    if (ok != 0) {
        if ((int)g_cur_settlement_rec->sold >= 0)
            g_native_buy[g_cur_settlement_rec->sold] = 0;
        if ((int)g_cur_settlement_rec->bought >= 0)
            g_native_buy[g_cur_settlement_rec->bought] = 0;
        sort_16x16(0x10, buy_order, g_native_buy);
        if (goods != buy_order[15] && goods != buy_order[14]) {
            popup_say(0, g_goods_name_word[buy_order[15]]);
            popup_say(1, g_goods_name_word[buy_order[14]]);
            popup_say(2, g_goods_name_word[buy_order[13]]);
            if (human != 0)
                popi("BRING", g_cur_tribe);
        }
        if (human != 0 && (int)g_cur_settlement_rec->refused == -2) {
            popi("BADHAGGLE3", g_cur_tribe);
            goto done;
        }
        if (goods < 0) {
            if (human != 0)
                popi("DEFICIT", g_cur_tribe);
            goto done;
        }
        n = 0;
        for (i = 0; n < 3 && i < 0x10; i++) {
            goods = order[0x10 - (i + 1)];
            if (goods == 0xf)
                continue;
            if (goods == 0)
                continue;
            if (goods == 0xe)
                continue;
            if (goods == 0xd)
                continue;
            popup_say(n, g_goods_name_word[goods]);
            pick[n] = 0x10 - (i + 1);
            n++;
        }
        if (human != 0)
            answer = popi("BUYWHICH", g_cur_tribe);
        else {
            answer = 1;
            best = -9999;
            for (i = 0; i < 3; i++) {
                good = order[0x10 - (i + 1)];
                tier = g_goods_value[nation][good];
                if (tier > best) {
                    best = tier;
                    answer = i + 1;
                }
            }
        }
        if (answer < 1 || answer > 3)
            goto done;
        sel = pick[answer - 1];
        goods = order[sel];
        if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST)
            g_cargo_amount = g_cargo_amount >> 2;
        price = 0xc8;
        if (goods >= 8)
            price += (4 - g_cur_tribe_rec->level) * 0x32;
        if (goods >= 7)
            price += g_goods_value[nation][goods] * (g_game.difficulty * 2 + 0xf);
        price += rand_range(0, price);
        price -= g_native_sell[sel] * 4;
        price += pissed(g_cur_tribe, nation) * 4;
        price = (int)((long)price * g_cargo_amount / 100L);
        roll = rand_range(0, 2);
        price += (g_game.difficulty + roll) * 10;
        price = max_int_1030_ead6(0x32, price);
        popup_say(0, g_goods_name_word[goods]);
        g_text_args[2] = g_cargo_amount;
        set_text_arg_l_1030_ec2f(3, g_nations[nation].gold);
        popup_say(1, (g_units[unit].type >= UNIT_SHIP_FIRST &&
                                    g_units[unit].type <= UNIT_SHIP_LAST) ? g_misc_words[MISC_HOLDS] : g_misc_words[MISC_WAGONS]);
        STRCPY(msg, "BUY0");
        round = 0;
        do {
            again = 0;
            limit = *max_ptr_1030_f4a6((t2 = 0xa, &t2), (t1 = price >> 1, &t1));
            step = *max_ptr_1030_f4a6((t4 = 1, &t4), (t3 = price >> 2, &t3));
            g_text_args[0] = price;
            g_text_args[1] = limit;
            msg[3] = (char)(round + 0x30);
            answer = 1;
            if (human != 0)
                answer = popi(msg, g_cur_tribe);
            switch (answer) {
            case 1:
                if (price <= g_nations[nation].gold) {
                    g_nations[nation].gold -= price;
                    g_cur_settlement_rec->bought = (char)goods;
                    if (goods == 9)
                        g_cur_settlement_rec->bought = -1;
                    g_cur_tribe_rec->goods_drift[goods] -= g_cargo_amount;
                    stow_cargo(unit, goods, g_cargo_amount);
                    mood = rand_range(0, price / 0x19 + 1);
                    piss_off(g_cur_tribe, nation, -mood, 0);
                } else {
                    g_text_args[0] = g_nations[nation].gold;
                    if (human != 0)
                        popi("NOTENOUGH", g_cur_tribe);
                    piss_off(g_cur_tribe, nation, 1, 0);
                }
                break;
            case 2:
                margin = g_native_sell[sel] / 0x19 + 8;
                roll = rand_range(0, margin);
                if (price > 10 && g_game.difficulty + 1 < roll) {
                    price = price - step;
                    price = max_int_1030_ead6(10, price);
                    if (rand_range(1, 8 - g_game.difficulty) == 1)
                        piss_off(g_cur_tribe, nation, 1, 0);
                    if ((get_treaty(nation, tribe_nation) & 0x40) != 0) {
                        round = 1;
                        again = 1;
                    }
                } else {
                    piss_off(g_cur_tribe, nation, 2, 0);
                    if ((get_treaty(nation, tribe_nation) & 0x40) != 0) {
                        g_cur_settlement_rec->refused = -2;
                        popi("BADHAGGLE2", g_cur_tribe);
                    }
                }
                break;
            case 3:
                break;
            }
        } while (again != 0);
    }
done:
    g_cur_settlement_rec->anger[nation] =
        max_int_1030_ead6(0, g_cur_settlement_rec->anger[nation]);
}

