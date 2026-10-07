/* negotiate_tribe: candidate for 1030:383d (4942 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3
 * symbol: negotiate_tribe
 * name-source: sibling-build
 * size-source: 4942, not the census 4933. The last nine bytes are an unreachable second epilogue;
 * the next function starts straight after them.
 *
 * A NATIVE UNIT CALLING ON A EUROPEAN COLONY OR WAGON (after first contact), everything except the trade screen. It returns 1 when
 * the visit did something and 0 when nothing came of it. The caller passes the visiting nation, the
 * tribe's own nation number (4..11), the unit, and the direction it stepped in.
 *
 * FIRST CONTACT is the whole first half. A nation that has never met this tribe -- relation bit 0x20
 * clear -- gets the meeting: the bit is set both ways, the tribe's anger with that nation is
 * capped at 20, a drum plays (a different one for each of the first two tribes), the village tile is
 * redrawn and a woodcut runs, and INDIANWELCOME offers peace. Accepting calls
 * `indian_make_peace`; refusing costs a hundred points of alarm and shows INDIANSHUN. Either way
 * the visit ends there, with the tribe's mood for that nation set to 2.
 *
 * AFTER THAT the routine looks at what is actually on the square. A EUROPEAN mover goes on only
 * when the top unit on the target square is a wagon train: the direction is reversed, the mover is taken as the
 * wagon and the unit on the target square becomes the visitor. A NATIVE mover picks up whatever colony and wagon
 * are on the square it steps into. A unit
 * carrying flag 8, or standing on a native village square, does nothing.
 *
 * WHETHER THE VILLAGE IS IN THE MOOD is three tests. Its grudge against the nation (+0xa of the
 * settlement record, one word per nation) at 0x80 or more makes it angry, and so does a mood of 1;
 * tension at 0x4b or more makes it hostile and ends the visit outright; and an angry village only
 * lets the visit continue when `rand_range(1, 0x80)` beats the grudge over 0x80. A GIFT is then
 * offered when `max(tension - 25, 0) * 4 + grudge` comes in under `rand_range(1, 0x148)` -- but
 * never while angry or hostile, and never once tension reaches 0x32, which sets the mood to 2 instead.
 *
 * THE VILLAGE MAY BEG. When it pays more for food than it asks (SEG25 0x10 against 0x30) and the
 * colony has 75 or more, INDIANBEGFOOD asks for half of it. Handing it over empties the grudge and
 * lowers alarm by five -- doubled for a capital, and five more for every step still above 0x46 --
 * and can turn into a gift anyway on a `rand_range(0, difficulty)` of zero. Refusing raises the
 * grudge by half and the alarm by `(difficulty + 1) / 2 + 1`, doubled for a capital and halved again
 * when the village was feeling generous.
 *
 * A GIFT IS ONE OF THREE THINGS. A village whose own mission belongs to the visiting nation
 * (settlement +5, low nibble) converts one of its people, `level + 2` in sixteen and twice that when
 * the mission is an expert's (MISSION_EXPERT, 0x10 of that byte): INDIANSCONVERT and a new colonist with speciality 0x1b in the colony. Failing that, a
 * colony short of food (25 or less) is topped up to 75 with INDIANGIVEFOOD. Otherwise the village
 * gives goods: the sixteen goods are sorted by what it asks for them, anything the colony already
 * has within ten of its warehouse is pushed to the bottom (its key set to 1), and one of the top three is picked -- never
 * food, and never goods 7 from a tribe below level 2. The amount is the asking price plus five,
 * clamped by `100 / (value + 1)`, by 100, by the warehouse space left, and floored at 5 and then 2.
 *
 * WITH NO GIFT the village ASKS for something instead. Every goods in the colony is scored as
 * `value to this nation * min(100, stock)` -- with horses worth more when the tribe has few herds and
 * muskets worth more with the difficulty and the lower the tribe's level -- and the best is demanded, halved on one roll in `difficulty + 1`. INDIANCITY
 * offers it; giving it empties the grudge and lowers alarm by `4 * score / 100`, and muskets or
 * horses also upgrade the visiting unit (or go to the tribe's own stock when it cannot be upgraded).
 * Refusing adds 0x80 to the grudge. With no colony but a wagon train present the same thing happens
 * to the wagon's first cargo, through INDIANWAGONS.
 *
 * WHAT THE BYTES SETTLED: `dir ^ 4` is written `dir = dir ^ 4`, not `dir ^= 4` (rule 60). The
 * settlement's own byte at +6 of the unit and its owner nibble at +5 are SIGNED and need explicit
 * `(int)` casts, or Borland compares them a byte at a time. `get_treaty` and
 * `treaty_on` return `int` here, not `unsigned char` -- the shipped stores AX
 * straight into the local. And the last two branches are structured `if`s and not `goto`s: the
 * shipped tests are long conditionals, which is rule 12's other half.
 *
 * RENAMED 2026-09-26 from visit_tribe_village: the Mac build's negotiate_tribe(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (callee SlideIcon; callee get_cargo; callee get_cargo_amount; callee pissed; callee popup_say), the runner-up gets 4, and the arity agrees (4).
 */

#include "colonization.h"

#include "control.h"
#include "tribes.h"
#include "jobs.h"
#include "settle.h"
#include "woodcut.h"
#include "nations.h"
#include "goods.h"
#include "units.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */
#include "classes.h"

extern void set_tribe(int tribe);
extern int pissed(int tribe, int i);
extern void popup_say(int slot, int word);
extern void force_mood(int mood);
extern void set_mood(int mood);
extern void force_map(int x, int y);

extern int popi(char *name, int tribe);
extern void piss_off(int tribe, int nation, int delta, int unused);
extern int is_village(int x, int y);
extern int top_unit_in_square(int x, int y);

extern void set_village(int s);

extern void set_center(int x, int y, int mode);
extern void MapWin_SlideIcon(void *w, int u, int speed, int a, int x0, int y0, int x1, int y1);
extern void DrawMapRegion(int x, int y, int w, int h, int flag);
extern void set_colony(int colony);
extern int get_warehouse(void);

extern void sort_16x16(int n, int *payload, int *key);
extern int stack_check(int u, int kind);
extern int is_armed(int u);
extern int is_horse(int u);
extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);
extern int unload_cargo(int u, int slot);
extern void end_unit_turn(int u);

/* reached through the five-byte NOP form, so external (rule 47) */
extern int get_treaty(int nation_a, int nation_b);
extern int treaty_on(int nation, int other, int bits);

extern int get_name(int nation);
extern void indian_supply_and_demand(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int indian_make_peace(int nation, int tribe);

int negotiate_tribe(register int nation, int tribe_nation, register int unit, int dir)
{
    int ret = 0;
    int flags;
    int tension;
    int ux;
    int uy;
    int tx;
    int ty;
    int saved;
    int wagon = -1;
    int colony = -1;
    int anger;
    int angry;
    int hostile;
    int roll;
    int unation;
    int gift = 0;
    int goods;
    int amount;
    int i;
    int have;
    int bestval;
    int val;
    int want;
    int answer;
    int delta;
    int generous;
    int diff;
    int give;
    int begged = 0;
    int ok;
    int chance;
    int cap;
    int sound;
    int t1;
    int t2;
    int order[16];

    saved = unit;
    set_tribe(tribe_nation - 4);
    tension = pissed(g_cur_tribe, nation);
    flags = get_treaty(nation, tribe_nation);
    if ((flags & 0x20) == 0) {
        flags = treaty_on(nation, tribe_nation, 0x20);
        g_cur_tribe_rec->anger[nation] =
            min_int_1030_eb20(g_cur_tribe_rec->anger[nation], 0x14);
        tension = pissed(g_cur_tribe, nation);
        popup_say(0, get_adjective(tribe_nation));
        popup_say(1, g_level[g_cur_tribe_rec->level].settlements);
        set_text_arg_b_1030_eb6a(0, g_tribe_settlement_count[g_cur_tribe]);
        if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
            sound = 5;
            if (g_cur_tribe == 0)
                sound = 7;
            if (g_cur_tribe == 1)
                sound = 6;
            if (g_game.turn_counter >= 0x14 || g_sound_music == 0 || g_cur_tribe < 2)
                force_mood(sound);
            else
                set_mood(sound);
            force_map(g_units[unit].x, g_units[unit].y);
            switch (g_cur_tribe) {
            case 0:
                woodcut(WOODCUT_THE_INCA_NATION);
                break;
            case 1:
                woodcut(WOODCUT_THE_AZTEC_EMPIRE);
                break;
            default:
                woodcut(WOODCUT_MEETING_THE_NATIVES);
                break;
            }
            answer = popi("INDIANWELCOME", g_cur_tribe);
        } else
            answer = 1;
        if (answer != 2)
            flags = indian_make_peace(nation, tribe_nation);
        else {
            piss_off(g_cur_tribe, nation, 0x64, 0);
            popup_say(0, get_name(tribe_nation));
            popi("INDIANSHUN", g_cur_tribe);
        }
        ret = 1;
        g_cur_tribe_rec->mood[nation] = 2;
        goto out;
    }
    ux = g_units[unit].x;
    uy = g_units[unit].y;
    tx = g_dir_dx[dir] + ux;
    ty = g_dir_dy[dir] + uy;
    if ((g_units[unit].flags2 & 8) != 0)
        goto out;
    if (is_village(ux, uy) >= 0)
        goto out;
    unation = (unsigned char)(g_units[unit].flags & UNIT_NATION_MASK);
    unit = top_unit_in_square(tx, ty);
    if (unation < NATION_EUROPEAN_COUNT) {
        if (g_units[unit].type == UNIT_WAGON_TRAIN) {
            wagon = saved;
            dir = dir ^ 4;
            if (unit < 0)
                goto out;
        } else
            goto out;
    } else {
        if (unit >= 0 && g_units[unit].type == UNIT_WAGON_TRAIN)
            wagon = unit;
        colony = find_colony(tx, ty);
        unit = saved;
    }
    ux = g_units[unit].x;
    uy = g_units[unit].y;
    tx = g_dir_dx[dir] + ux;
    ty = g_dir_dy[dir] + uy;
    if ((int)g_units[unit].home < 0)
        goto out;
    set_village((int)g_units[unit].home);
    anger = g_cur_settlement_rec->anger[nation];
    angry = (anger >= 0x80);
    hostile = (tension >= 0x4b);
    if (g_cur_tribe_rec->mood[nation] == 1)
        angry = 1;
    if (hostile != 0)
        goto out;
    if (angry != 0) {
        roll = rand_range(1, 0x80);
        if (anger - 0x80 > roll)
            goto out;
    }
    roll = rand_range(1, 0x148);
    generous = ((*max_ptr_1030_f4a6((t2 = tension - 0x19, &t2), (t1 = 0, &t1)) << 2)
                + anger <= roll);
    gift = generous;
    if (angry != 0 || hostile != 0)
        gift = 0;
    if (gift != 0 && tension >= 0x32) {
        gift = 0;
        g_cur_tribe_rec->mood[nation] = 2;
    }
    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN && colony >= 0) {
        set_center(tx, ty, 0);
        MapWin_SlideIcon(&g_map_window, saved, 0xc0, -1, ux, uy, tx, ty);
        DrawMapRegion(tx, ty, 2, 2, 1);
        if (rand_range(0, 8) == 0) {
            sound = 5;
            if (g_cur_tribe == 0)
                sound = 7;
            if (g_cur_tribe == 1)
                sound = 6;
            set_mood(sound);
        }
    }
    indian_supply_and_demand();
    diff = g_native_buy[GOODS_FOOD] - g_native_sell[GOODS_FOOD];
    if (diff > 0 && hostile == 0 && colony >= 0) {
        set_colony(colony);
        if (g_cur_colony_rec->stock[GOODS_FOOD] >= 0x4b) {
            give = g_cur_colony_rec->stock[GOODS_FOOD] >> 1;
            if (rand_range(1, 100) <= diff) {
                ret = 1;
                popup_say(0, get_adjective(tribe_nation));
                set_name_slot(1, g_cur_colony_rec->name);
                set_text_arg_i_1030_eb46(0, give);
                set_text_arg_i_1030_eb46(1, g_cur_colony_rec->stock[GOODS_FOOD]);
                if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
                    force_map(g_units[unit].x, g_units[unit].y);
                    answer = popi("INDIANBEGFOOD", g_cur_tribe);
                } else
                    answer = (nation == NATION_SPAIN) ? 1 : 2;
                if (answer == 2) {
                    g_cur_colony_rec->stock[GOODS_FOOD] -= give;
                    g_cur_settlement_rec->anger[nation] = 0;
                    delta = -5;
                    if ((g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL) != 0)
                        delta = delta * 2;
                    while (pissed(g_cur_tribe, nation) + delta > 0x46)
                        delta -= 5;
                    piss_off(g_cur_tribe, nation, delta, 0);
                    begged = 1;
                    if (gift == 0 && rand_range(0, g_game.difficulty) != 0)
                        goto out;
                    gift = 1;
                    g_cur_tribe_rec->mood[nation] = 2;
                } else {
                    g_cur_settlement_rec->anger[nation] +=
                        g_cur_settlement_rec->anger[nation] >> 1;
                    delta = ((g_game.difficulty + 1) >> 1) + 1;
                    if ((g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL) != 0)
                        delta = delta * 2;
                    if (generous != 0)
                        delta = delta >> 1;
                    piss_off(g_cur_tribe, tribe_nation, delta, 0);
                    if (g_cur_tribe_rec->mood[nation] == 2)
                        g_cur_tribe_rec->mood[nation] = 0;
                    if (gift != 0)
                        goto out;
                }
            }
        }
    }
    if (gift != 0) {
        if (colony < 0)
            goto out;
        g_cur_tribe_rec->mood[nation] = 2;
        set_colony(colony);
        cap = get_warehouse();
        g_cur_settlement_rec->anger[nation] = 0;
        popup_say(0, get_adjective(tribe_nation));
        if ((g_cur_settlement_rec->mission & 0xf) == nation) {
            chance = g_cur_tribe_rec->level + 2;
            if ((g_cur_settlement_rec->mission & 0x10) != 0)
                chance = chance * 2;
            if (rand_range(0, 0xf) < chance) {
                if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
                    set_name_slot(0, g_cur_colony_rec->name);
                    popi("INDIANSCONVERT", g_cur_tribe);
                }
                unit = create_unit(UNIT_COLONISTS, g_cur_colony_rec->nation, g_cur_colony_rec->x,
                                             g_cur_colony_rec->y);
                if (unit < 0)
                    goto out;
                g_units[unit].spec = JOB_CONVERT;
                ret = 1;
                goto out;
            }
        }
        if (g_native_sell[GOODS_FOOD] > g_native_buy[GOODS_FOOD] && begged == 0 &&
            g_cur_colony_rec->stock[GOODS_FOOD] <= 0x19) {
            give = 0x4b - g_cur_colony_rec->stock[GOODS_FOOD];
            set_text_arg_i_1030_eb46(0, give);
            g_cur_colony_rec->stock[GOODS_FOOD] += give;
            if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
                force_map(g_units[unit].x, g_units[unit].y);
                popi("INDIANGIVEFOOD", g_cur_tribe);
            }
            ret = 1;
            goto out;
        }
        g_native_sell[GOODS_FOOD] = 0;
        for (i = 0; i < 0x10; i++) {
            order[i] = (unsigned char)i;
            if (g_cur_colony_rec->stock[i] > cap - 10)
                g_native_sell[i] = 1;
        }
        sort_16x16(0x10, order, g_native_sell);
        roll = rand_range(1, 3);
        do {
            goods = order[0x10 - roll];
            ok = 1;
            if (goods == GOODS_SILVER && g_cur_tribe_rec->level < LEVEL_ADVANCED)
                ok = 0;
            if (goods == GOODS_FOOD)
                ok = 0;
            if (ok == 0)
                roll++;
        } while (ok == 0);
        amount = g_native_sell[0x10 - roll] + 5;
        amount = min_int_1030_eb20(amount,
                     100 / (g_goods_value[g_cur_colony_rec->nation][goods] + 1));
        amount = min_int_1030_eb20(amount, 0x64);
        amount = max_int_1030_ead6(amount, 5);
        amount = min_int_1030_eb20(amount, cap - g_cur_colony_rec->stock[goods]);
        amount = max_int_1030_ead6(2, amount);
        set_name_slot(1, g_cur_colony_rec->name);
        set_text_arg_i_1030_eb46(0, amount);
        popup_say(2, g_goods_name_word[goods]);
        g_cur_colony_rec->stock[goods] += amount;
        if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
            force_map(g_units[unit].x, g_units[unit].y);
            popi("INDIANGIVESTUFF", g_cur_tribe);
        }
        ret = 1;
        goto out;
    }
retry:
    if (g_cur_tribe_rec->mood[nation] == 2 || (colony < 0 && wagon < 0)) {
        if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
            popup_say(0, get_adjective(tribe_nation));
            popup_say(1, get_name(nation));
            popi("INDIANCOMMENT", g_cur_tribe);
        }
        g_cur_settlement_rec->anger[nation] = 0;
        goto out;
    }
    if (colony >= 0) {
        set_colony(colony);
        goods = -1;
        bestval = 0;
        for (i = 0; i < 0x10; i++) {
            have = min_int_1030_eb20(0x64, g_cur_colony_rec->stock[i]);
            want = g_goods_value[nation][i];
            if (i == 8)
                want += 0xa - g_cur_tribe_rec->horse_herds;
            if (i == 0xf)
                want += rand_range(1, 4) + (g_game.difficulty + (4 - g_cur_tribe_rec->level));
            val = want * have;
            if (val > bestval) {
                bestval = val;
                goods = i;
                amount = have;
            }
        }
        if (goods >= 0) {
            g_cur_tribe_rec->mood[nation] = 1;
            if (rand_range(0, g_game.difficulty + 1) == 0)
                amount = amount >> 1;
            popup_say(0, get_adjective(nation));
            popup_say(1, get_adjective(tribe_nation));
            popup_say(2, g_goods_name_word[goods]);
            set_name_slot(3, g_cur_colony_rec->name);
            set_text_arg_i_1030_eb46(0, amount);
            if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
                sound = 5;
                if (g_cur_tribe == 0)
                    sound = 7;
                if (g_cur_tribe == 1)
                    sound = 6;
                force_mood(sound);
                force_map(g_units[unit].x, g_units[unit].y);
                answer = popi("INDIANCITY", g_cur_tribe);
            } else {
                answer = 2;
                if (goods == GOODS_MUSKETS)
                    answer = 1;
                if (stack_check(top_unit_in_square(tx, ty), 0xa) != 0)
                    answer = 1;
            }
            if (answer == 2) {
                g_cur_settlement_rec->anger[nation] = 0;
                delta = bestval * 4 / 100;
                delta = 0 - delta;
                while (pissed(g_cur_tribe, nation) + delta > 0x46)
                    delta -= 5;
                piss_off(g_cur_tribe, nation, delta, 0);
                g_cur_colony_rec->stock[goods] -= amount;
                if (goods == GOODS_MUSKETS) {
                    if (is_armed(unit) == 0)
                        g_units[unit].type++;
                    else
                        g_cur_tribe_rec->muskets++;
                }
                if (goods == GOODS_HORSES) {
                    if (is_horse(unit) == 0)
                        g_units[unit].type = g_units[unit].type + 2;
                    else
                        g_cur_tribe_rec->horses += 0x32;
                    g_cur_tribe_rec->horse_herds++;
                }
            } else
                g_cur_settlement_rec->anger[nation] += 0x80;
        } else {
            colony = -1;
            goto retry;
        }
        ret = 1;
        goto out;
    }
    if (wagon >= 0) {
        g_cur_tribe_rec->mood[nation] = 1;
        goods = get_cargo(wagon, 0);
        amount = get_cargo_amount(wagon, 0);
        popup_say(0, get_adjective(nation));
        popup_say(1, get_adjective(tribe_nation));
        popup_say(2, g_goods_name_word[goods]);
        set_text_arg_i_1030_eb46(0, amount);
        if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN && amount != 0) {
            sound = 5;
            if (g_cur_tribe == 0)
                sound = 7;
            if (g_cur_tribe == 1)
                sound = 6;
            force_mood(sound);
            set_center(tx, ty, 0);
            MapWin_SlideIcon(&g_map_window, saved, 0xc0, -1, ux, uy, tx, ty);
            DrawMapRegion(tx, ty, 2, 2, 1);
            force_map(g_units[unit].x, g_units[unit].y);
            answer = popi("INDIANWAGONS", g_cur_tribe);
        } else
            answer = 1;
        if (answer == 1) {
            g_cur_settlement_rec->anger[nation] = 0;
            delta = g_goods_value[nation][goods] * amount * 4 / 100;
            delta = 0 - delta;
            piss_off(g_cur_tribe, nation, delta, 0);
            unload_cargo(wagon, 0);
        } else
            g_cur_settlement_rec->anger[nation] += 0x80;
        ret = 1;
    }
out:
    if (ret != 0 && (unsigned char)(g_units[saved].flags & UNIT_NATION_MASK) >= 4)
        end_unit_turn(saved);
    return ret;
}
