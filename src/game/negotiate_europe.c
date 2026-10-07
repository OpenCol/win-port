/* PORT: g_leader_traits is LeaderFace[] (bytes a, b, c): [n][0..2] read as .a/.b/.c, signed as
   the corpus's signed char view read them (W1). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 605, 611, 629, 642, 684, 686, 701, 704, 715, 763, 769, 771, 801, 805, 817, 827, 831, 841 of the imported file. */
/* PORT: this file's own declarations of text_dialog_sel_b0 removed: the header declares it from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 773 of the imported file. */
/* negotiate_europe: candidate for 1030:5240 (9035 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: negotiate_europe
 * name-source: sibling-build
 * size-source: 9035, not the census 9027. The last eight bytes are the four-entry jump table the
 * greeting-sound switch reads through `CS:[BX+0x7583]`; `blockade_speed` follows.
 *
 * TWO EUROPEAN POWERS MEET, AND EVERY WORD THEY CAN SAY TO EACH OTHER. This is the whole
 * diplomatic exchange: the first contact, the tribute demand, the offer of peace, the four things
 * the other side can ask for in return, and the mercenary contract against a third power. Nothing
 * else in the game talks to `text_dialog_sel_b0` this many times.
 *
 * IT OPENS BY PRICING THE RELATIONSHIP. A loop over land regions 1..14 compares the two sides'
 * strength region by region: where the other nation has colonies and this one's strength beats the
 * other's units there, the ratio becomes a DEBT it could be paid off with; everywhere else it becomes part of the DEMAND. Every
 * colony either side owns is then run through `siege_check_2`: a colony of theirs adds
 * twice its defence to the demand and, when it is weakly held, sets the hostile flag; a colony of
 * this nation subtracts from it. The raw figure is then bent by the difficulty level, the year, the
 * size of this nation, the treasury it can actually see, and the King's-support flag, and finally
 * rounded to a multiple of fifty.
 *
 * THEN IT COUNTS FRIENDS. Every tribe and every other European power that this nation is on terms
 * with adds to a score, and the score decides whether the demand is pressed at all -- a nation with
 * allies is left alone. The same sweep remembers ONE third party as `other`, and that is who the
 * accusation ("APOSTATES" for a European, "HEATHEN" for a tribe) and later the mercenary offer are
 * about.
 *
 * THE CONVERSATION ITSELF is a chain of `text_dialog_sel_b0` calls, each of which is a modal message
 * with buttons, and each answer steers the next one:
 *   the greeting, which is AHOY for a ship, FIRST for a first meeting and MEEK or MANLY after that;
 *   the accusation about the third party, which on agreement drops the hostility and, on a coin flip, the demand;
 *   PIRACY, which on agreement sends every privateer this nation owns back to Europe;
 *   SIEGES, which on agreement pulls every armed unit standing next to their colonies away;
 *   TRIBUTE, which takes the gold outright;
 *   WANTSTUFF, which moves a cargo of one goods between two colonies instead;
 *   and then either PROVOKE, WARMANLY or RID if it is still hostile, or WORTHY -- the peace offer --
 *   if it is not.
 * Accepting peace opens a second menu: NOTHINGWITHDRAW when there is nothing to withdraw from,
 * WITHDRAW when there is and the debt covers it, MAYBEWITHDRAW with a price when it does not, GIFTS
 * when they ask for money instead, and MILITARY, which builds a Popup of every nation and tribe this
 * one knows and sells a war against whichever is chosen.
 *
 * A SHIPPED BUG, and a clear one. The goods transfer that WANTSTUFF agrees to indexes the two
 * colonies with the LEFT-OVER LOOP COUNTER rather than with the goods it just named: the message
 * says "we want twenty tobacco" from `best_goods`, and the code then moves twenty of goods `i`,
 * where `i` is whatever the friend-counting loops left behind -- four, in every ordinary game. The
 * shipped `MOV AX,DI` is unambiguous, and it is the same register the four following loops use.
 *
 * WHAT THE BYTES SETTLED, five spellings.
 *   ONE LOOP COUNTER IS A REGISTER VARIABLE and four different loops share it, while the region loop
 *   at the top keeps its own on the frame. Without the extra `register int` the frame comes out two
 *   bytes short, and it is the only local Borland put in DI.
 *   The strength comparisons are BYTE compares of a byte against a truncated product, so the source
 *   reads `g_nation_strength[a] < (unsigned char)(g_nation_strength[b] * 3)`; an `int` comparison there is two
 *   bytes longer and unsigned/signed the wrong way round.
 *   `hostile = f406 = 0` assigns in that order -- the shipped stores `f406` first, and the other
 *   spelling reverses the two `MOV`s (rule 59).
 *   Two of the halvings are compound `>>= 1` (a single `SAR WORD PTR`) and the rest are the plain
 *   `x = x >> 1` load/shift/store, and the shipped uses each exactly where it does (rule 60).
 *   And the rank words at SEG20:0xd9d6 are compared both ways: signed where they are weighed against
 *   a tribe's strength byte, unsigned where two nations' ranks are compared with each other, so the
 *   source casts at the three unsigned sites.
 *
 * `g_units_expelled` WAS `g_far_1182`, named 2026-09-25 from its three sites, which agree.
 * THIS function sets it, in the one place it is set: when `f452` holds, every LAND unit of
 * nation `b` standing next to a square owned by nation `a` is detached and sent home to `b`'s
 * start position. So the latch records that an expulsion happened. `foreign_turn` clears it
 * at the top of a nation's turn, which makes it per-turn, and `plan_move` spends it with
 * `if (g_units_expelled != 0) flags = 0;` -- cancelling the whole set of region bits (0x10)
 * it had just built up from region state, unclaimed regions, a rival colony and a garrison.
 *
 * NAMED AFTER WHAT THE SETTER DOES, not after why the AI then ignores its regions, which
 * nothing here establishes. Its DGROUP neighbours `g_land_claim` (0x117e) and `g_sea_claim`
 * (0x1180) are region BITMASKS built with `|= 1 << region`; this is a plain 0/1 flag, so it is
 * adjacent to them and not one of them.
 *
 * RENAMED 2026-09-26 from european_encounter: the Mac build's negotiate_europe(short, short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 11 of its Mac call edges already bound land on this function (callee has_father; callee item; callee land_region_at; callee minimax; callee on_map), the runner-up gets 5, and the arity agrees (5).
 */

#include "colonization.h"

#include "difficul.h"
#include "fathers.h"
#include "nations.h"
#include "control.h"
#include "woodcut.h"
#include "gameflag.h"
#include "tribes.h"
#include "sounds.h"
#include "units.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern Nation g_nations[];               /* SEG20:0xc2f4, 0x13c bytes each */

extern GameState g_game;   /* SEG20:0x7782 */

extern int g_rank_ids[4];                /* SEG25:0x0 -- 0x6 is [3] */

extern int g_units_expelled;                   /* DGROUP:0x1182, declared */

extern void Popup_ctor(Popup *p, int n);
extern void Popup_dtor(Popup *p, int n);
extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern char *strings(int n);
extern int get_treaty(int nation_a, int nation_b);
extern int treaty_off(int nation, int other, int mask);
extern int treaty_on(int nation, int other, int m);

extern "C" void sound_play(int n);   /* PORT: C linkage, as defined (W2) */
extern void set_europe(int n);
extern void set_colony(int colony);
extern void set_tribe(int tribe);
extern int siege_check_2(int *best_out, int *own_out, int *total_out, int nation);
extern int land_region_at(int x, int y);

extern int has_father(int nation, int bit);

extern int find_close_colony(int x, int y, int nation, int land);

extern int get_warehouse(void);
extern int pissed(int tribe, int n);
extern int get_name(int nation);

extern void popup_say(int slot, int word);
extern void force_mood(int mood);
extern void stack_ship(int u);
extern void relocate_stacked_units(int u, int x, int y);
extern void relocate_unit(int u, int x, int y);
extern void piss_off(int tribe, int nation, int delta, int f);

extern int is_colony(int x, int y);

extern void space_string(char *buf);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void unfortify(int nation, int owner);
extern "C" void set_ranks(void);
extern "C" void negotiate_ai(int nation, int other);

extern "C" void great(int slot, char *name, int n);
extern "C" void meek(int slot, int mode);

/* g_str_usa is a global: all seven appends push DGROUP:0x1350, one copy among the module's
   initialised data (tools/measure_data_coverage.py). As a literal it compiles to the
   same code bytes but seven copies in the literal pool. The module DEFINES it, here:
   segment 7's _DATA holds it after the local-array templates of the functions before this
   one and before the module's vtables (tools/check_module_data.py, 2026-10-02). */
char g_str_usa[] = "USA";                /* DGROUP:0x1350 */

extern "C" int negotiate_europe(int nation, int their_nation, int u, int dir, int flag)
{
    register int i;
    int ret = 0;
    int owed = 0;
    int demand = 0;
    int hostile = 0;
    int f406 = 0;
    int f408 = 0;
    int act = 0;
    int f40c = 0;
    int at_war;
    int last_turn;
    int g;
    int amount;
    int f416;
    int score;
    int other;
    int answer;
    int answer2;
    int ci;
    int cdef_nation;
    int cdef_value;
    int cdef_b;
    int cdef_c;
    int best_goods = -1;
    int best_amount;
    int value;
    int qty;
    int best_value;
    int src_colony;
    int dst_colony;
    int tx;
    int ty;
    int ux;
    int uy;
    int nx;
    int ny;
    int region;
    int adj_dir;
    int cap;
    int limit;
    int f44c;
    int saveu;
    int nships;
    int f452;
    int mycol;
    int hiscol;
    int f458;
    int price;
    int f45c = 0;
    int f45e;
    int f460;
    int f462 = 0;
    char buf[0x50];
    char buf2[0x14];
    Popup p(0x800);

    set_ranks();
    if (nation >= NATION_EUROPEAN_COUNT || g_nation_text[nation].control != CONTROL_HUMAN) {
        negotiate_ai(nation, their_nation);
        ret = 1;
        goto out;
    }
    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
        goto out;
    act = flag;
    if (!(get_treaty(nation, their_nation) & 0x20)) {
        act = 1;
        woodcut(WOODCUT_MEETING_FELLOW_EUROPEANS);
    }
    if (g_game.war_started[their_nation] + 0x10 <= g_game.turn_counter) {
        act = 1;
        treaty_off(nation, their_nation, 0x10);
    }
    f460 = get_treaty(nation, their_nation) & 0x10;
    if (act == 0)
        goto out;
    switch (their_nation) {
    case 0:
        sound_play(SOUND_ANTHEM_ENGLAND);
        break;
    case 1:
        sound_play(SOUND_ANTHEM_FRANCE);
        break;
    case 2:
        sound_play(SOUND_ANTHEM_SPAIN);
        break;
    case 3:
        sound_play(SOUND_ANTHEM_SPAIN);
        break;
    }
    if (g_nations[their_nation].flags & 4)
        f462 = 1;
    set_europe(nation);
    ret = 1;
    last_turn = g_game.war_started[their_nation];
    g_game.war_started[their_nation] = g_game.turn_counter;
    for (g = 1; g < 0xf; g++) {
        if (g_region_colony_count[their_nation][g] > (g_nation_colony_count[their_nation] > 1 ? (unsigned char)1
                                                            : (unsigned char)0)
            && g_nation_region_strength[nation][g] > g_region_unit_count[their_nation][g]) {
            owed += (g_nation_region_strength[nation][g] / (g_region_unit_count[their_nation][g] + 1))
                    << (g_game.difficulty != DIFFICULTY_DISCOVERER ? 1 : 2);
        } else {
            if (g_nation_region_strength[nation][g] != 0 && g_nation_region_strength[their_nation][g] != 0)
                hostile = 1;
            if (g_nation_region_strength[their_nation][g] != 0 && g_region_population[nation][g] > 4)
                hostile = 1;
            if (g_region_colony_count[nation][g] != 0) {
                if (g_region_colony_count[their_nation][g] > 1)
                    demand += g_nation_region_strength[their_nation][g] - g_nation_region_strength[nation][g];
                else
                    demand += g_nation_region_strength[their_nation][g];
            } else {
                amount = g_nation_region_strength[their_nation][g] - g_nation_region_strength[nation][g];
                demand += amount >> (g_region_colony_count[their_nation][g] != 0 ? 1 : 2);
            }
        }
    }
    mycol = 0;
    hiscol = 0;
    f458 = 0;
    for (ci = 0; g_game.colony_count > ci; ci++) {
        set_colony(ci);
        if ((char)g_cur_colony_rec->nation != (char)nation
            && (char)g_cur_colony_rec->nation != (char)their_nation)
            continue;
        cdef_value = siege_check_2(&cdef_nation, &cdef_b, &cdef_c, their_nation);
        if (cdef_nation == their_nation) {
            demand += cdef_value * 2;
            if (cdef_b == 0 || cdef_c > 1) {
                if (owed != 0)
                    owed--;
                hostile = 1;
            }
            region = land_region_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
            if (g_region_colony_count[their_nation][region] == 0)
                cdef_value = cdef_value * 2;
            hiscol += cdef_value;
            f458 = 1;
        }
        if (cdef_nation == nation) {
            mycol += cdef_value;
            demand -= cdef_c * 2;
        }
    }
    if ((unsigned char)nation == g_rank_ids[3] && g_game.turn_counter >= 0x50 && g_nation_colony_count[nation] > 3
        && g_nation_colony_count[their_nation] > 1)
        f406 = 1;
    at_war = g_nations[their_nation].rel[nation] & 2;
    if (f406 != 0
        || (at_war != 0
            && g_nation_strength[nation] < (unsigned char)(g_nation_strength[their_nation] * 3))) {
        hostile = 1;
        owed = 0;
        demand = minimax(demand, g_game.difficulty * 0xc8 + 0x64, 0x26ac);
    }
    if (f460 != 0)
        hostile = 1;
    if (owed != 0) {
        hostile = 0;
        f408 = 0;
    }
    if (has_father(nation, FATHER_BENJAMIN_FRANKLIN) && nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
        hostile = f406 = 0;
        if (last_turn < 0)
            last_turn = 0;
    } else if (has_father(their_nation, FATHER_BENJAMIN_FRANKLIN)) {
        hostile = f406 = 0;
        if (last_turn < 0)
            last_turn = 0;
    }
    if (at_war == 0 && 100 - g_game.difficulty * 10 > g_game.turn_counter) {
        hostile = f406 = 0;
        if (last_turn < 0)
            last_turn = 0;
    }
    demand = (g_game.difficulty * 10 + 0x50) * demand / 100;
    if (at_war != 0)
        demand = demand * 2;
    else if (last_turn < 0)
        demand = demand >> 2;
    else {
        if (g_game.turn_counter < 0x32)
            demand = demand >> 1;
        else if (g_game.turn_counter < 0x64)
            demand = demand - (demand >> 2);
        if (g_nation_colony_count[nation] <= 2 && g_nation_population[nation] < 8)
            demand = demand >> 1;
    }
    demand = minimax((g_game.difficulty + 1) * demand >> 3, 0, 0x190) * 0x32;
    if (f460 != 0)
        demand += g_game.difficulty * 0x1f4 + 0x1f4;
    if (demand > g_cur_nation_rec->gold && demand < g_cur_nation_rec->gold * 2
        && g_cur_nation_rec->gold >= 300L)
        demand = (int)(g_cur_nation_rec->gold / 0x32) * 0x32;
    if (has_father(nation, FATHER_BENJAMIN_FRANKLIN))
        demand = demand >> 1;
    if (demand == 0 || g_nation_strength[nation] > (unsigned char)(g_nation_strength[their_nation] * 3))
        hostile = 0;
    if (hostile != 0)
        f408 = 1;
    f416 = hostile;
    if ((unsigned char)(g_units[u].flags & 0xf) == (unsigned char)their_nation) {
        dst_colony = find_close_colony(g_units[u].x, g_units[u].y, their_nation, -1);
        tx = g_units[u].x + g_dir_dx[dir];
        ty = g_units[u].y + g_dir_dy[dir];
        ci = find_colony(tx, ty);
        if (ci >= 0 && dst_colony >= 0) {
            set_colony(dst_colony);
            cap = get_warehouse();
            src_colony = ci;
            set_colony(ci);
            best_value = 0;
            limit = demand >> 1;
            amount = demand / 10;
            limit += (g_game.difficulty - 2) * amount;
            for (i = 0; i < 0x10; i++) {
                qty = g_cur_colony_rec->stock[i];
                qty = min_int_1030_eb20(cap - g_colonies[dst_colony].stock[i], qty);
                value = g_goods_value[their_nation][i] * qty;
                if (value < limit)
                    continue;
                if (value >= best_value) {
                    best_value = value;
                    best_goods = i;
                    best_amount = qty;
                }
            }
        }
    }
    score = 0;
    if (at_war != 0)
        score = -2;
    other = -1;
    for (i = 0; i < 8; i++) {
        set_tribe(i);
        if (g_cur_tribe_rec->flags & 0x80)
            continue;
        if (pissed(i, their_nation) >= TRIBE_ATTITUDE_WAR
            || (get_treaty(nation, i + 4) & 2)) {
            if ((int)g_military_power[their_nation] < g_tribe_strength[i])
                score++;
            score++;
            if (pissed(i, nation) < TRIBE_ATTITUDE_WAR
                && !(get_treaty(nation, i + 4) & 2))
                other = i + 4;
        }
    }
    for (i = 0; i < 4; i++) {
        if (i == nation)
            continue;
        if (i == their_nation)
            continue;
        if ((get_treaty(their_nation, i) & 0x60) == 0x20) {
            if ((int)g_military_power[i] * 4 > (int)g_military_power[their_nation])
                score++;
            if ((unsigned)g_military_power[i] > (unsigned)g_military_power[their_nation])
                score++;
            if (get_treaty(nation, i) & 0x40)
                other = i;
        }
    }
    score -= ((signed char)g_leader_traits[their_nation].a);
    if ((unsigned)g_military_power[their_nation] > (unsigned)g_military_power[nation])
        score--;
    if (f406 == 0) {
        if (((get_treaty(nation, their_nation) & 0x40) ? 0 : 1) < score) {
            hostile = 0;
            if (rand_range(0, 1) != 0)
                demand = 0;
        }
    }
    if (flag == 0 && hostile == 0 && owed != 0
        && (get_treaty(nation, their_nation) & 0x40))
        goto out;
    if (hostile != 0 && (get_treaty(nation, their_nation) & 0x40)
        && (at_war != 0 || (unsigned)g_military_power[nation] > (unsigned)g_military_power[their_nation])) {
        f40c = 1;
        hostile = 0;
        f408 = 0;
    }
    if (f408 == 0)
        STRCPY(buf2, "MEEK");
    else
        STRCPY(buf2, "MANLY");
    buf[0] = 0;
    say_string(buf, g_difficulty_words[g_game.difficulty]);
    space_string(buf);
    STRCAT(buf, g_nation_text[nation].leader);
    set_name_slot(0, buf);
    set_name_slot(1, g_nation_text[their_nation].country);
    great(2, "KINGS", their_nation);
    great(3, "DEEDS", their_nation);
    STRCPY(buf, "HELLO");
    force_mood(4);
    if (!(get_treaty(nation, their_nation) & 0x20)) {
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
            STRCAT(buf, "AHOY");
        else
            STRCAT(buf, "FIRST");
    } else
        STRCAT(buf, buf2);
    if (f462 != 0)
        STRCPY(buf, "HELLOUSA");
    text_dialog_sel_b0(buf, their_nation);
    if (other >= 0 && f460 == 0 && (get_treaty(nation, other) & 0x20)) {
        popup_say(0, get_adjective(other));
        if (other < NATION_EUROPEAN_COUNT) {
            STRCPY(buf, "APOSTATES");
            meek(1, f408 == 0 ? 1 : 0);
            if (f462 != 0)
                STRCAT(buf, g_str_usa);
        } else {
            STRCPY(buf, "HEATHEN");
            if (f462 != 0) {
                STRCAT(buf, g_str_usa);
                set_name_slot(0, g_nation_text[their_nation].country);
            } else
                popup_say(1, get_name(other));
        }
        answer = text_dialog_sel_b0(buf, their_nation);
        if (answer == 2) {
            hostile = 0;
            if (rand_range(0, 1) != 0)
                demand = 0;
            if (other < NATION_EUROPEAN_COUNT) {
                treaty_off(nation, other, 0x40);
                g_nations[other].rel[nation] |= 2;
            } else
                piss_off(other - 4, nation, 0x64, 0);
        }
    }
    if ((get_treaty(their_nation, nation) & 0x80) && f460 == 0
        && g_unit_tally[nation][UNIT_PRIVATEER] != 0) {
        great(0, "LEADER", their_nation);
        popup_say(1, get_adjective(nation));
        set_name_slot(2, g_nation_text[their_nation].country);
        meek(3, f408);
        STRCPY(buf, "PIRACY");
        if (f462 != 0) {
            STRCAT(buf, g_str_usa);
            set_name_slot(0, g_nation_text[their_nation].country);
        }
        answer = text_dialog_sel_b0(buf, their_nation);
        if (answer == 2) {
            saveu = u;
            nships = 0;
            for (u = 0; g_game.unit_count > u; u++) {
                if ((unsigned char)(g_units[u].flags & 0xf) != (unsigned char)nation)
                    continue;
                if (g_units[u].type != UNIT_PRIVATEER)
                    continue;
                if (g_units[u].x != (unsigned char)(nation - 0x14))
                    nships++;
                stack_ship(u);
                relocate_stacked_units(u, nation - 0x14, nation - 0x14);
                g_units[u].dest_x = g_nations[nation].start_x;
                g_units[u].dest_y = g_nations[nation].start_y;
            }
            g_nations[their_nation].rel[nation] &= 0x7f;
            if (nships != 0 && rand_range(0, nships) != 0)
                hostile = 0;
            demand = demand / (nships + 1);
            u = saveu;
        }
    }
    if (f460 == 0) {
        if (g_pop_by_nation[their_nation] >> 2 <= mycol
            || (mycol > 0xc && rand_range(0, 4) == 0)) {
            great(0, "LEADER", their_nation);
            popup_say(1, get_adjective(nation));
            popup_say(2, get_adjective(their_nation));
            meek(3, f408);
            STRCPY(buf, "SIEGES");
            if (f462 != 0) {
                STRCAT(buf, g_str_usa);
                set_name_slot(0, g_nation_text[their_nation].country);
            }
            answer = text_dialog_sel_b0(buf, their_nation);
            if (answer == 2) {
                hostile = 0;
                demand -= mycol * 0x64;
                demand = max_int_1030_ead6(0, demand);
                f45c = 1;
                for (u = 0; g_game.unit_count > u; u++) {
                    if ((unsigned char)(g_units[u].flags & 0xf) != (unsigned char)nation)
                        continue;
                    if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
                        continue;
                    if (g_unit_types[g_units[u].type].attack <= 1)
                        continue;
                    ux = g_units[u].x;
                    uy = g_units[u].y;
                    f452 = 0;
                    if (on_map(ux, uy) == 0)
                        continue;
                    for (adj_dir = 0; f452 == 0 && adj_dir < 8; adj_dir++) {
                        nx = g_dir_dx[adj_dir] + ux;
                        ny = g_dir_dy[adj_dir] + uy;
                        if (is_colony(nx, ny) == their_nation)
                            f452 = 1;
                    }
                    if (f452 != 0) {
                        relocate_unit(u, nation - 0x14, nation - 0x14);
                        g_units[u].dest_x = g_nations[nation].start_x;
                        g_units[u].dest_y = g_nations[nation].start_y;
                    }
                }
            }
        }
    }
    if (demand != 0 && f408 == 1 && demand < g_cur_nation_rec->gold) {
        great(0, "LEADER", their_nation);
        popup_say(1, get_adjective(nation));
        set_name_slot(2, g_nation_text[their_nation].country);
        set_text_arg_i_1030_eb46(0, demand);
        STRCPY(buf, "TRIBUTE");
        if (f462 != 0) {
            STRCAT(buf, g_str_usa);
            set_name_slot(0, g_nation_text[their_nation].country);
        }
        answer = text_dialog_sel_b0(buf, their_nation);
        if (answer == 2) {
            g_cur_nation_rec->gold -= demand;
            g_nations[their_nation].gold += demand;
            hostile = 0;
        }
        demand = 0x3e7;
    }
    if (f408 != 0 && demand != 0x3e7 && best_goods >= 0) {
        great(0, "LEADER2", their_nation);
        set_text_arg_i_1030_eb46(0, best_amount);
        popup_say(1, g_goods_name_word[best_goods]);
        popup_say(2, get_adjective(their_nation));
        STRCPY(buf, "WANTSTUFF");
        if (f462 != 0) {
            STRCAT(buf, g_str_usa);
            set_name_slot(0, g_nation_text[their_nation].country);
        }
        answer = text_dialog_sel_b0(buf, their_nation);
        if (answer == 2) {
            g_colonies[src_colony].stock[i] -= best_amount;
            g_colonies[dst_colony].stock[i] += best_amount;
            hostile = 0;
        }
    }
    if (hostile != 0 && (get_treaty(nation, their_nation) & 0x40) && demand > 0x64) {
        text_dialog_sel_b0((void*)("PROVOKE"), their_nation);
        treaty_off(nation, their_nation, 0x40);
    } else if (hostile != 0 && demand == 0x3e7) {
        great(0, "LEADER2", their_nation);
        set_name_slot(1, g_nation_text[their_nation].country);
        force_mood(4);
        text_dialog_sel_b0((void*)("WARMANLY"), their_nation);
        treaty_off(nation, their_nation, 0x40);
    } else if (hostile != 0) {
        great(0, "LEADER2", their_nation);
        set_name_slot(1, g_nation_text[their_nation].country);
        STRCPY(buf, "RID");
        if (f462 != 0)
            STRCAT(buf, g_str_usa);
        text_dialog_sel_b0(buf, their_nation);
    }
    f44c = 0;
    if (hostile == 0 && !(get_treaty(nation, their_nation) & 0x40)) {
        great(0, "LEADER2", their_nation);
        popup_say(1, get_adjective(nation));
        popup_say(2, get_adjective(their_nation));
        if (f462 != 0)
            answer = 1;
        else
            answer = text_dialog_sel_b0((void*)("WORTHY"), their_nation);
        if (answer == 1) {
            treaty_on(nation, their_nation, 0x40);
            g_game.war_started[their_nation] = g_game.turn_counter + 0x10;
            STRCPY(buf, "PEACE");
        } else {
            if (at_war == 0) {
                amount = minimax(owed * 2 - 4, 0,
                                 (int)(g_nations[their_nation].gold / 100)) * 0x64;
                if (amount != 0) {
                    f408 = 0;
                    STRCPY(buf2, "MEEK");
                    set_text_arg_i_1030_eb46(0, amount);
                    answer = text_dialog_sel_b0((void*)("GIVECASH"), their_nation);
                    if (answer == 1) {
                        treaty_on(nation, their_nation, 0x40);
                        g_nations[their_nation].gold -= amount;
                        g_cur_nation_rec->gold += amount;
                    }
                }
            }
            if (!(get_treaty(nation, their_nation) & 0x40)) {
                STRCPY(buf, "WAR");
                STRCAT(buf, buf2);
                great(0, "LEADER2", their_nation);
                set_name_slot(1, g_nation_text[their_nation].country);
                force_mood(4);
                text_dialog_sel_b0(buf, their_nation);
            }
        }
    } else if (hostile == 0) {
        STRCPY(buf, "OLDPEACE");
        f44c = 1;
    }
    if (get_treaty(nation, their_nation) & 0x40) {
        unfortify(nation, their_nation);
        unfortify(their_nation, nation);
    }
    if (hostile == 0 && (get_treaty(nation, their_nation) & 0x40)) {
        popup_say(0, get_adjective(nation));
        popup_say(1, get_adjective(their_nation));
        STRCAT(buf, buf2);
        if (f44c != 0) {
            popup_say(2, g_difficulty_words[g_game.difficulty]);
            set_name_slot(3, g_nation_text[nation].leader);
        }
        if (f462 != 0) {
            STRCPY(buf, "PEACEUSA");
            set_name_slot(0, g_nation_text[their_nation].country);
            popup_say(1, get_name(nation));
        }
        answer = text_dialog_sel_b0(buf, their_nation);
        if (answer == 2) {
            f452 = 0;
            if (f458 == 0)
                text_dialog_sel_b0((void*)("NOTHINGWITHDRAW"), their_nation);
            else if (owed != 0 && f40c == 0) {
                text_dialog_sel_b0((void*)("WITHDRAW"), their_nation);
                f452 = 1;
            } else {
                price = (g_game.difficulty * 0x19 + 0x32) * hiscol;
                if (at_war != 0)
                    price = price * 2;
                if (f416 != 0)
                    price += price >> 1;
                if (f45c != 0)
                    price -= mycol * 0x32;
                if (has_father(nation, FATHER_BENJAMIN_FRANKLIN))
                    price >>= 1;
                price = max_int_1030_ead6(0x64, price);
                popup_say(0, get_adjective(their_nation));
                if (price > g_cur_nation_rec->gold || f40c != 0)
                    text_dialog_sel_b0((void*)("NOTWITHDRAW"), their_nation);
                else {
                    set_text_arg_i_1030_eb46(0, price);
                    answer2 = text_dialog_sel_b0((void*)("MAYBEWITHDRAW"), their_nation);
                    if (answer2 == 1) {
                        g_cur_nation_rec->gold -= price;
                        g_nations[their_nation].gold += price;
                        f452 = 1;
                    }
                    if (answer2 == 2) {
                        f45e = g_nation_strength[nation] + g_nation_strength[their_nation];
                        if (at_war != 0)
                            f45e = f45e * 2;
                        if (rand_range(0, f45e) <= g_nation_strength[nation]) {
                            text_dialog_sel_b0((void*)("WITHDRAW"), their_nation);
                            f452 = 1;
                        } else {
                            STRCPY(buf, "WAR");
                            if (f462 != 0)
                                STRCPY(buf2, "MANLY");
                            STRCAT(buf, buf2);
                            great(0, "LEADER2", their_nation);
                            set_name_slot(1, g_nation_text[their_nation].country);
                            force_mood(4);
                            text_dialog_sel_b0(buf, their_nation);
                            treaty_off(nation, their_nation, 0x40);
                        }
                    }
                }
            }
            if (f452 != 0) {
                g_units_expelled = 1;
                for (u = 0; g_game.unit_count > u; u++) {
                    if ((unsigned char)(g_units[u].flags & 0xf) != (unsigned char)their_nation)
                        continue;
                    if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
                        continue;
                    ux = g_units[u].x;
                    uy = g_units[u].y;
                    f452 = 0;
                    if (on_map(ux, uy) == 0)
                        continue;
                    for (adj_dir = 0; f452 == 0 && adj_dir < 8; adj_dir++) {
                        nx = g_dir_dx[adj_dir] + ux;
                        ny = g_dir_dy[adj_dir] + uy;
                        if (is_colony(nx, ny) == nation)
                            f452 = 1;
                    }
                    if (f452 != 0) {
                        relocate_unit(u, their_nation - 0x14, their_nation - 0x14);
                        g_units[u].dest_x = g_nations[their_nation].start_x;
                        g_units[u].dest_y = g_nations[their_nation].start_y;
                    }
                }
            }
        }
        if (answer == 3) {
            if (has_father(nation, FATHER_BENJAMIN_FRANKLIN) && rand_range(0, 2) == 0)
                owed++;
            amount = minimax(owed, 0, (int)(g_nations[their_nation].gold / 100)) * 0x64;
            if (amount > 0) {
                set_text_arg_i_1030_eb46(0, amount);
                text_dialog_sel_b0((void*)("GIFTS"), their_nation);
                g_nations[their_nation].gold -= amount;
                g_cur_nation_rec->gold += amount;
            } else if (f416 != 0) {
                force_mood(4);
                treaty_off(nation, their_nation, 0x40);
                text_dialog_sel_b0((void*)("PROVOKE"), their_nation);
            } else
                text_dialog_sel_b0((void*)("THREATS"), their_nation);
        }
        if (answer == 4) {
            show_plain_dialog_1030_ebb7(&p, (void*)("MILITARY"), 0);
            amount = 0;
            for (i = 0; i < 0xc; i++) {
                if (i >= 4) {
                    set_tribe(i);
                    if (g_cur_tribe_rec->flags & 0x80)
                        continue;
                    if (!(get_treaty(nation, i) & 0x20))
                        continue;
                } else {
                    if (i == nation)
                        continue;
                    if (i == their_nation)
                        continue;
                    if (!(get_treaty(nation, i) & 0x20))
                        continue;
                }
                Popup_item(&p, strings(get_name(i)), i);
                amount = 1;
            }
            if (amount == 0)
                goto out;
            answer = popup_run_thunk_1030_ebe7(&p);
            if (answer > 0) {
                other = answer - 1;
                popup_say(0, get_name(other));
                if (!(get_treaty(their_nation, other) & 0x20)) {
                    text_dialog_sel_b0((void*)("NOCONTACT"), their_nation);
                    goto done;
                }
                if (!(get_treaty(their_nation, other) & 0x40)) {
                    text_dialog_sel_b0((void*)("ALREADYSMITE"), their_nation);
                    goto done;
                }
                if (other >= NATION_EUROPEAN_COUNT) {
                    set_tribe(other);
                    amount = minimax((int)((long)g_tribe_strength[g_cur_tribe]
                                           * (g_nations[nation].gold / 0x32) * 3 / 0x32),
                                     0xa, 0xc8) * 0x32;
                    if (has_father(nation, FATHER_BENJAMIN_FRANKLIN))
                        amount = amount >> 1;
                    popup_say(0, get_adjective(other));
                    set_text_arg_i_1030_eb46(0, amount);
                    answer = text_dialog_sel_b0((void*)("SMITEINDIANS"), their_nation);
                } else {
                    amount = minimax((int)((long)(unsigned)(g_military_power[other]
                                                            + g_nation_strength[other])
                                           * (g_nations[nation].gold / 0x32) / 0x32),
                                     0xa, 0xc8) * 0x32;
                    if (has_father(nation, FATHER_BENJAMIN_FRANKLIN))
                        amount = amount >> 1;
                    popup_say(0, get_adjective(other));
                    set_text_arg_i_1030_eb46(0, amount);
                    answer = text_dialog_sel_b0((void*)("SMITEEUROPE"), their_nation);
                }
                if (answer == 1) {
                    if (amount > g_cur_nation_rec->gold)
                        text_dialog_sel_b0((void*)("UNFORTUNATE"), their_nation);
                    else {
                        treaty_off(their_nation, other, 0x40);
                        if (other < NATION_EUROPEAN_COUNT)
                            g_nations[other].rel[their_nation] |= 2;
                        else
                            treaty_on(their_nation, other, 2);
                        popup_say(0, get_name(their_nation));
                        popup_say(1, get_name(other));
                        force_mood(4);
                        text_dialog_sel_b0((void*)("MERCENARY"), their_nation);
                        g_cur_nation_rec->gold -= amount;
                        g_nations[their_nation].gold += amount;
                    }
                }
            }
        }
    }
done:
    if (f40c != 0)
        g_nations[their_nation].rel[nation] |= 8;
out:
    if (get_treaty(nation, their_nation) & 0x40) {
        amount = (4 - g_game.difficulty) * 2 + 4;
        if (has_father(nation, FATHER_BENJAMIN_FRANKLIN))
            amount >>= 1;
        g_nations[their_nation].timer[nation] = amount;
    }
    return ret;
}
