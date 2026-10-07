/* PORT: this file's own declarations of build_another_colony, cargo_to_europe, fill_docks removed: the header declares them from the definition (W1 step 3). */
/* foreign_country: candidate for 1028:3555 (5942 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3
 * symbol: foreign_country
 * name-source: sibling-build
 *
 * WHAT A COMPUTER NATION DOES IN EUROPE each turn: it is paid, it decides what it is short of, it
 * recruits and equips colonists, it buys ships, and then it loads every ship in port and sends it
 * west. `foreign_turn` calls it once per computer nation.
 *
 * THE SUBSIDY comes first. The nation's colony count plus `(SEG20:0x778c - 1500) / 50`, doubled
 * once that figure passes 1700, times the difficulty, half as much again on Governor and twice
 * as much on Viceroy, and then times four -- that is what lands in the treasury before anything is
 * bought. Nothing at all before turn twenty.
 *
 * WHAT IT WANTS is worked out from four things: whether it has a college, whether any of its
 * ships is still in Europe, how many privateers and frigates the OTHER three European nations have
 * between them (tally columns 0x10 and 0x11, weighted four to one for frigates), and how its
 * armed-ship count at DGROUP:0x4b64 ranks against the others.
 * Two "I need a warship" flags come out of that, one for each of the two thresholds at DGROUP:0x4c0b
 * and 0x4c0c, and either of them is enough to top the treasury up to a floor from SEG20:0xd858 or
 * 0xd85e. A nation that has run out of one resource entirely gets the floor at 0xd846 instead.
 *
 * RECRUITING goes down a fixed list, stopping at the first success: a warship for each of the two
 * flags (and if that one fails the whole turn ends), then a frigate (entry 5) with fewer than eight
 * armed ships, a galleon (3), a merchantman (2) with cargo capacity under twelve, a caravel (1)
 * with capacity two or less, and a privateer (4) -- all SHIPS from `g_recruit`; the second
 * argument (35, 50, 20, 20, 30) is one `try_to_purchase` never reads. A second pass then buys a colonist outright when it has a spare pioneer allowance,
 * at `(7 - difficulty + recruit_count) * 20` (the purchase count at +6 of the nation record) less a
 * share of the crosses already earned.
 *
 * EQUIPPING runs twice over the units waiting in Europe, once for the specialists and once for the
 * rest, and turns a plain colonist into a soldier (50 muskets), a pioneer (100 tools) or a
 * missionary (free, once, and only on a turn divisible by seven). Handing muskets to a specialist
 * SWAPS that specialist into the recruit pool and takes the ordinary colonist out of it, so the
 * expensive people stay on the dock. A soldier bought while the nation has a college has a chance,
 * scaled by its own two counters, of coming out as a veteran.
 *
 * TROOPS: with the treasury over the price the nation recruits a colonist off the docks and fits
 * him out as a dragoon, and it keeps recruiting while there is room aboard (`cap - n_troops`); an
 * artillery piece comes first when it has none and room for six. Muskets and horses are shuffled between the two
 * market stocks first so that neither runs out while the other is full.
 *
 * SAILING: every ship in port sells its cargo (muskets and horses go back into the stock rather
 * than being sold), fills up with whatever the goods-wanted table at SEG24:0x65e asks for most, and
 * sets sail. A ship that was just bought sails immediately.
 *
 * A SHIPPED BUG: `u` is read one past the end of the unit walk. The `for` that looks for a non-ship
 * unit leaves `u` negative, and the very next statement tests `g_units[u].flags2` -- so the "wait a turn"
 * flag is taken from whatever lies before the unit array.
 *
 * WHAT THE BYTES SETTLED: each of the two warship conditions ends in a term the compiler folds to
 * constant FALSE. Written without it the last test falls straight into `MOV AX,1`; with it Borland
 * emits `jnz <true>` and a separate `jmp <false>`, which is what the shipped has, and the trailing
 * group also becomes the false target for the three tests before it. The comparisons against
 * DGROUP:0x4c0b and 0x4b5c stay in AL and so need `(unsigned char)` round the whole left side
 * (rule 83); DGROUP:0x4c06 is a SIGNED byte array; the crosses divisor is `(long)need + 1`, widened
 * before the increment and not after; the goods-wanted table is compared as a signed byte against
 * the low half of an int; and the three early exits are `goto`, not `return` (rule 12).
 *
 * `g_game.royal_forces[ROYAL_MAN_O_WAR]++` used to be `g_77e0++` over a separate scalar at
 * SEG20:0x77e0, which is element 2 of the King's array at 0x77dc. The line reads as what it
 * does once it is written that way: a man-o-war is taken off the map and put back into the
 * King's fleet count.
 *
 * THREE GOLD FLOORS, at SEG20:0xd846, 0xd858 and 0xd85e, read as one statement:
 *
 *     if (no_ships)       gold = max(gold, g_recruit[1].price);
 *     if (want_privateer) gold = max(gold, g_recruit[4].price);
 *     if (want_frigate)   gold = max(gold, g_recruit[5].price);
 *
 * THE THREE ARE PRICES IN THE EUROPE RECRUIT TABLE, `g_recruit` at SEG20:0xd83c -- six
 * six-byte entries whose price is at +3, so 0xd846, 0xd858 and 0xd85e are entries 1, 4 and 5.
 * The function itself is the confirmation, forty lines below: `want_a` buys
 * `try_to_purchase(5, ...)` and `want_b` buys `try_to_purchase(4, ...)`, the same two indexes, and
 * the floor for `poor` is entry 1, the one bought when the cargo capacity is at most 2. The
 * AI is topped up to exactly the price of the thing it is about to buy.
 *
 * The AI is TOPPED UP to a floor that depends on its situation -- it is given money rather
 * than earning it. `poor` is `g_nation_ship_count[nation] < 1`, so that floor is for a
 * nation with no ships; `want_a` and `want_b` are the two purchase intents computed just
 * above. The recruit table says which is which: init_data fills entry 1 with type 13
 * (Caravel, 1000), entry 4 with type 16 (Privateer, 2000) and entry 5 with type 17 (Frigate,
 * 5000), so want_a is the frigate purchase and want_b the privateer one, and the floor for a
 * nation with no ships is a caravel's price. The locals are named so (2026-09-26).
 *
 * `g_ai_military_target` WAS `g_long_4b68`, and the reason it could be named is that the
 * evidence came from the CONSUMER side rather than the filler. The worklist had it marked LEAVE
 * because every other counter in its DGROUP block is filled by `collect_stats` and this
 * one is not -- true, and the wrong place to look. Nothing in the image stores to 0x4b68 at all:
 * the only two instructions naming it are the `push word 0x4b68` of the save read and the save
 * write, so it is not a counter the game maintains but a per-nation byte the SAVE FILE supplies
 * (field 24, `docs/formats/save-file.md`).
 *
 * Both reads are here and both are the same decision, which is what makes the name safe:
 * `rand_range(0, tally[nation][UNIT_SOLDIERS] + tally[nation][UNIT_DRAGOONS]) <= target` makes a
 * colonist just armed as a soldier a VETERAN (`spec = JOB_SOLDIER`). The nation's CURRENT soldier-and-dragoon count is what the roll
 * is taken against, so the byte is the size that count is being pushed towards: the bigger it
 * is, the longer the nation keeps arming. Named after the test and not the intent, as
 * `include/aicol.h` requires -- it is a target the military is compared against, and this file
 * does not establish who sets it or why one nation's differs from another's.
 *
 * RENAMED 2026-09-26 from ai_europe_turn: the Mac build's foreign_country(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller foreign_turn; callee get_cargo; callee set_colony; callee set_europe; callee top_unit_in_square), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "royal.h"
#include "difficul.h"
#include "jobs.h"
#include "building.h"
#include "gameflag.h"
#include "units.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

                                  /* 6 bytes at SEG20:0xd83c -- the base `setup_purchase`
                                     writes, one byte BEFORE the 0xd83d the readers used */

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_europe(int n);
extern void set_colony(int colony);
extern int has_building(int bit);
extern int top_unit_in_square(int x, int y);
extern void delete_unit(int u);
extern int next_unit(int u);

extern int stack_check(int u, int kind);
extern int unit_from_docks(int spec);
extern int job_of_unit(int u);
extern int any_spec(int job);
extern int europe_ask_price(int g);
extern void traffic_from_europe(int g, int n);

extern void traffic_to_europe(int g, int n);
extern int europe_bid_price(int g);
extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);
extern int unload_cargo(int u, int slot);
extern void add_gold(int nation, long amount);
extern void set_course_for_new_world(int u);
extern int cargo_from_europe(int u, int g, int want);
/* reached through the five-byte NOP form, so external (rule 47) */

/* same object: 4-byte `push cs; call ` (rule 47) */

extern int try_to_purchase(int kind, int cost);

void foreign_country(int nation)
{
    int again;
    int goods;
    int cost;
    int bought_tools;
    int has_land;
    int bought;
    int got;
    int price;
    int slot;
    int want_pioneers;
    int want;
    int first;
    int best;
    int ties;
    int low_rank;
    int phase;
    int cap;
    int has_college;
    int n_troops;
    int room;
    int threat;
    int specialist;
    int base;
    int tax;
    int swap;
    int n_artillery;
    int no_ships;
    int want_privateer;
    int want_frigate;
    int crowded;
    int loaded;
    int skip;
    int t1;
    int t2;
    register int u;
    register int i;

    bought_tools = 0;
    has_land = 0;
    bought = 0;
    has_college = 0;
    n_artillery = 0;
    crowded = 0;
    loaded = 0;
    skip = 0;
    set_europe(nation);
    base = g_nation_colony_count[nation];
    base += (g_game.year - 0x5dc) / 0x32;
    if (g_game.turn_counter < 0x14)
        base = 0;
    if (g_game.year >= 0x6a4)
        base <<= 1;
    tax = g_game.difficulty * base;
    if (g_game.difficulty == DIFFICULTY_GOVERNOR)
        tax += tax >> 1;
    if (g_game.difficulty == DIFFICULTY_VICEROY)
        tax <<= 1;
    tax <<= 2;
    g_cur_nation_rec->gold += tax;
    for (i = 0; g_game.colony_count > i && has_college == 0; i++) {
        set_colony(i);
        if (g_cur_colony_rec->nation != (unsigned char)nation)
            continue;
        if (has_building(BUILDING_COLLEGE) == 0)
            continue;
        has_college = 1;
    }
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) != 0) {
        for (u = top_unit_in_square(nation - 0x14, nation - 0x14); u >= 0; u = next_unit(u)) {
            if (NATION(g_units[u]) != (unsigned char)nation)
                continue;
            if (g_units[u].type != UNIT_MAN_O_WAR)
                continue;
            delete_unit(u);
            g_game.royal_forces[ROYAL_MAN_O_WAR]++;
            break;
        }
    }
    no_ships = (g_nation_ship_count[nation] < 1);
    threat = 0;
    for (i = 0; i < 4; i++) {
        if (i == nation)
            continue;
        threat += g_unit_tally[i][0x10];
        threat += g_unit_tally[i][0x11] << 2;
    }
    threat = threat >> 2;
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) != 0)
        threat = 0;
    want_frigate = ((g_colonies_blockaded != 0 || g_colonies_besieged != 0) && threat != 0 &&
              (((unsigned char)(g_nation_colony_count[nation] >> 1) <= g_colonies_blockaded ||
                (g_pop_by_nation[nation] >> 1) <= g_pop_blockaded ||
                (g_game.turn_counter > 0xc8 && g_cur_nation_rec->gold >= 0x7d0L)) &&
               g_unit_tally[nation][0x11] == 0 &&
               g_unit_tally[g_game.player_nation][0x11] != 0 || 0));
    want_privateer = ((g_colonies_blockaded != 0 || g_colonies_besieged != 0) && threat != 0 && want_frigate == 0 &&
              (((unsigned char)(g_nation_colony_count[nation] >> 1) <= g_colonies_besieged ||
                (g_pop_by_nation[nation] >> 1) <= g_pop_besieged ||
                (g_game.turn_counter > 0x64 && g_cur_nation_rec->gold >= 0x3e8L)) &&
               g_unit_tally[nation][0x10] < 2 &&
               g_unit_tally[g_game.player_nation][0x10] != 0 || 0));
    if (no_ships != 0 && (long)g_recruit[1].price > g_cur_nation_rec->gold)
        g_cur_nation_rec->gold = g_recruit[1].price;
    if (want_privateer != 0 && (long)g_recruit[4].price > g_cur_nation_rec->gold)
        g_cur_nation_rec->gold = g_recruit[4].price;
    if (want_frigate != 0 && (long)g_recruit[5].price > g_cur_nation_rec->gold)
        g_cur_nation_rec->gold = g_recruit[5].price;
    best = ties = 0;
    for (i = 0; i < 4; i++)
        best = max_int_1028_e350(best, g_nation_armed_ships[i]);
    for (i = 0; i < 4; i++)
        if (g_nation_armed_ships[i] == best)
            ties++;
    low_rank = (want_frigate != 0 || g_nation_armed_ships[nation] < best || ties > 1);
    if ((unsigned char)(((g_nation_population[nation] >> 1) + g_nation_colony_count[nation] * 2) >> 1) >=
            g_nation_cargo_capacity[nation] &&
        (g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0)
        crowded = 1;
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0 &&
        ((g_nation_population[nation] >> 1) + g_nation_colony_count[nation]) >= g_nation_cargo_capacity[nation]) {
        got = 0;
        if (want_frigate != 0)
            got = try_to_purchase(5, 0x64);
        if (got == 0 && want_frigate != 0)
            goto out;
        if (want_privateer != 0)
            got = try_to_purchase(4, 0x64);
        if (got == 0 && want_privateer != 0)
            goto out;
        if (got == 0 && g_nation_armed_ships[nation] < 8 && rand_range(0, 1) != 0 && low_rank != 0)
            got = try_to_purchase(5, 0x23);
        if (got == 0 && rand_range(0, 3) != 0)
            got = try_to_purchase(3, 0x32);
        if (got == 0 && rand_range(0, 1) == 0 && g_nation_cargo_capacity[nation] < 0xc)
            got = try_to_purchase(2, 0x14);
        if (got == 0 && g_nation_cargo_capacity[nation] <= 2)
            got = try_to_purchase(1, 0x14);
        if (got == 0 && g_nation_armed_ships[nation] < 4 && rand_range(0, 3) == 0 && low_rank != 0 &&
            crowded == 0)
            got = try_to_purchase(4, 0x1e);
    }
    if (got == 0 && no_ships != 0)
        goto out;
    first = top_unit_in_square(nation - 0x14, nation - 0x14);
    n_artillery = stack_check(first, 0xc);
    if (n_artillery == 0 && (g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0 && (int)g_want_muskets > 0 &&
        rand_range(0, 3) == 0 && crowded == 0 && g_nation_cargo_capacity[nation] > 4) {
        try_to_purchase(0, 0x14);
        first = top_unit_in_square(nation - 0x14, nation - 0x14);
        n_artillery = stack_check(first, 0xc);
    }
    want_pioneers = build_another_colony(nation);
    for (u = first; u >= 0; u = next_unit(u))
        if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
            has_land = 1;
    if (g_game.turn_counter % 3 == 0)
        skip = 1;
    if ((g_units[u].flags2 & 0x20) != 0)
        skip = 1;
    if (g_nation_colony_count[nation] == 0)
        skip = 0;
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0 && has_land == 0 && crowded == 0 && skip == 0 &&
        ((int)g_colonies_with_room[nation] - g_nation_free_colonists[nation]) >= (g_nation_colony_count[nation] >> 1)) {
        price = (7 - g_game.difficulty + g_cur_nation_rec->recruit_count) * 0x14;
        price -= (int)((long)price * g_cur_nation_rec->crosses / ((long)g_cur_nation_rec->need + 1));
        if ((long)(*max_ptr_1028_e5ff((t2 = 0, &t2),
                   (t1 = g_nation_population[nation] * 0x3c - g_game.turn_counter * 2, &t1)) + price)
            <= g_cur_nation_rec->gold) {
            g_cur_nation_rec->gold -= price;
            slot = rand_range(0, 2);
            u = unit_from_docks(g_cur_nation_rec->recruit[slot]);
            if (u >= 0) {
                first = u;
                g_cur_nation_rec->recruit[slot] = fill_docks(0);
            }
        }
    }
    for (phase = 0; phase < 2; phase++) {
        first = top_unit_in_square(nation - 0x14, nation - 0x14);
        for (u = first; u >= 0; u = next_unit(u)) {
            if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) != 0)
                continue;
            if (job_of_unit(u) < 0)
                continue;
            if (any_spec((int)g_units[u].spec) != 0) {
                if (phase == 0)
                    continue;
            } else {
                if (phase != 0)
                    continue;
            }
            if (g_units[u].type == UNIT_PIONEERS)
                bought_tools |= want_pioneers;
            if (g_units[u].type == UNIT_COLONISTS && skip == 0) {
                specialist = 0;
                if (any_spec((int)g_units[u].spec) != 0)
                    specialist = 1;
                if (((int)g_want_muskets > 0 && rand_range(0, specialist + 1) == 0) ||
                    (want_pioneers != 0 && rand_range(0, specialist + 2) == 0 &&
                     g_game.turn_counter >= 0x64)) {
                    cost = europe_ask_price(0xf) * 0x32;
                    if (g_cur_nation_rec->muskets != 0)
                        cost = 0;
                    if ((long)cost <= g_cur_nation_rec->gold && crowded == 0) {
                        if (g_cur_nation_rec->muskets != 0)
                            g_cur_nation_rec->muskets--;
                        else
                            traffic_from_europe(0xf, 0x32);
                        g_cur_nation_rec->gold -= cost;
                        g_units[u].type = UNIT_SOLDIERS;
                        if (any_spec((int)g_units[u].spec) != 0) {
                            swap = 0x1c;
                            for (i = 0; i < 3; i++) {
                                if (any_spec(g_cur_nation_rec->recruit[i]) == 0) {
                                    swap = g_cur_nation_rec->recruit[i];
                                    g_cur_nation_rec->recruit[i] = g_units[u].spec;
                                    break;
                                }
                            }
                            g_units[u].spec = (char)swap;
                        }
                        want_pioneers = 0;
                        bought = 1;
                        g_want_muskets--;
                        if (has_college != 0 &&
                            rand_range(0, g_unit_tally[nation][1] + g_unit_tally[nation][4])
                                <= g_ai_military_target[nation])
                            g_units[u].spec = JOB_SOLDIER;
                        cost = europe_ask_price(8) * 0x32;
                        if (g_cur_nation_rec->horses >= 0x32)
                            cost = 0;
                        if ((long)cost <= g_cur_nation_rec->gold) {
                            g_cur_nation_rec->gold -= cost;
                            g_units[u].type = UNIT_DRAGOONS;
                            if (g_cur_nation_rec->horses >= 0x32)
                                g_cur_nation_rec->horses -= 0x32;
                            else
                                traffic_from_europe(8, 0x32);
                        }
                        continue;
                    }
                }
                if (g_units[u].type == UNIT_COLONISTS &&
                    want_pioneers - g_unit_tally[nation][2] > 0 &&
                    rand_range(0, 2) == 0 && bought_tools == 0) {
                    if (g_game.turn_counter >= 0x64 &&
                        rand_range(0, 2) <= g_unit_tally[nation][2])
                        continue;
                    if (any_spec((int)g_units[u].spec) != 0 &&
                        rand_range(0, 4) != 0)
                        continue;
                    cost = europe_ask_price(0xe) * 0x64;
                    if ((long)cost <= g_cur_nation_rec->gold) {
                        g_cur_nation_rec->gold -= cost;
                        traffic_from_europe(0xe, 0x64);
                        g_units[u].type = UNIT_PIONEERS;
                        bought_tools |= want_pioneers;
                        want_pioneers = 0;
                        bought = 1;
                        g_want_tools--;
                        continue;
                    }
                }
            }
            if (g_units[u].type == UNIT_COLONISTS && g_unit_tally[nation][3] == 0 &&
                g_game.turn_counter > 0x32 &&
                (g_game.turn_counter < 0xc8 || rand_range(0, 3) == 0) &&
                g_game.turn_counter % 7 == 0 &&
                (any_spec((int)g_units[u].spec) == 0 ||
                 rand_range(0, 7) == 0)) {
                g_units[u].type = UNIT_MISSIONARIES;
                g_unit_tally[nation][3]++;
            }
        }
    }
    n_troops = stack_check(first, 4);
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) != 0)
        n_troops += stack_check(first, 0xc);
    if (n_troops != 0 && crowded == 0 && skip == 0 && (g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
        cap = stack_check(first, 0xe);
        room = cap - n_troops;
        if (g_game.turn_counter > 0x50) {
            while (g_cur_nation_rec->horses / 0x32 > g_cur_nation_rec->muskets + 1) {
                g_cur_nation_rec->horses -= 0x32;
                g_cur_nation_rec->muskets++;
            }
            while (g_cur_nation_rec->muskets > g_cur_nation_rec->horses / 0x32 + 1) {
                g_cur_nation_rec->muskets--;
                g_cur_nation_rec->horses += 0x32;
            }
        }
        if (n_artillery == 0 && cap >= 6) {
            if (g_game.turn_counter >= 0x28)
                price = 0x3e8 - g_game.difficulty * 0x64;
            if (g_cur_nation_rec->artillery_credit != 0) {
                g_cur_nation_rec->artillery_credit--;
                price = 0;
            }
            if ((long)price <= g_cur_nation_rec->gold) {
                u = create_unit(UNIT_ARTILLERY, nation, nation - 0x14, nation - 0x14);
                if (u >= 0) {
                    first = u;
                    room--;
                    g_cur_nation_rec->gold -= price;
                }
            }
        }
        do {
            again = 0;
            price = (7 - (g_game.difficulty >> 1) + g_cur_nation_rec->recruit_count) * 0x14;
            price -= (int)((long)price * g_cur_nation_rec->crosses / ((long)g_cur_nation_rec->need + 1));
            if (g_cur_nation_rec->muskets == 0)
                price += europe_ask_price(0xf) * 0x32;
            if (g_game.turn_counter >= 0x64)
                price -= g_game.difficulty * price * 0xa / 0x64;
            if ((long)price <= g_cur_nation_rec->gold) {
                slot = rand_range(0, 2);
                u = unit_from_docks(g_cur_nation_rec->recruit[slot]);
                if (u < 0)
                    goto done;
                first = u;
                if (g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS) {
                    if (g_cur_nation_rec->muskets == 0) {
                        price -= europe_ask_price(0xf) * 0x32;
                        traffic_to_europe(0xf, 0x32);
                    } else
                        g_cur_nation_rec->muskets++;
                } else if (g_units[u].type == UNIT_PIONEERS) {
                    price -= europe_bid_price(0xe) * 0x64;
                    traffic_to_europe(0xe, 0x64);
                } else if (g_units[u].type == UNIT_SCOUTS)
                    g_units[u].type = UNIT_DRAGOONS;
                if (g_units[u].type != UNIT_DRAGOONS) {
                    g_units[u].type = UNIT_SOLDIERS;
                    if (any_spec((int)g_units[u].spec) != 0) {
                        swap = 0x1c;
                        for (i = 0; i < 3; i++) {
                            if (any_spec(g_cur_nation_rec->recruit[i]) == 0) {
                                swap = g_cur_nation_rec->recruit[i];
                                g_cur_nation_rec->recruit[i] = g_units[u].spec;
                                break;
                            }
                        }
                        g_units[u].spec = (char)swap;
                    }
                }
                g_cur_nation_rec->gold -= price;
                if (g_cur_nation_rec->muskets == 0)
                    traffic_from_europe(0xf, 0x32);
                else
                    g_cur_nation_rec->muskets--;
                if (has_college != 0 && (int)g_units[u].spec != JOB_SOLDIER &&
                    rand_range(0, g_unit_tally[nation][1] + g_unit_tally[nation][4])
                        <= g_ai_military_target[nation])
                    g_units[u].spec = JOB_SOLDIER;
                cost = europe_ask_price(8) * 0x32;
                if (g_game.turn_counter >= 0x64)
                    cost -= g_game.difficulty * cost * 0xa / 0x64;
                if (g_cur_nation_rec->horses >= 0x32)
                    cost = 0;
                if ((long)cost <= g_cur_nation_rec->gold)
                    g_cur_nation_rec->gold -= cost;
                g_units[u].type = UNIT_DRAGOONS;
                if (g_cur_nation_rec->horses >= 0x32)
                    g_cur_nation_rec->horses -= 0x32;
                else
                    traffic_from_europe(8, 0x32);
                g_cur_nation_rec->recruit[slot] = fill_docks(0);
                loaded = 1;
                again = 1;
                room -= g_unit_types[g_units[u].type].size;
            }
        } while (again != 0 && room > 0);
    }
done:
    want = (g_europe_land_units[nation] != 0) ? g_europe_land_units[nation] : 0;
    want += ((g_game.turn_counter & 1) != 0) ? 1 : 0;
    do {
        again = 0;
        for (u = top_unit_in_square(nation - 0x14, nation - 0x14); again == 0 && u >= 0;
             u = next_unit(u)) {
            if ((g_units[u].flags2 & 0x80) != 0 && g_units[u].type != UNIT_ARTILLERY)
                continue;
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
                g_units[u].home = -1;
                while (g_units[u].cargo_count != 0) {
                    goods = get_cargo(u, 0);
                    if (goods == 0xf) {
                        g_cargo_amount = get_cargo_amount(u, 0);
                        g_cargo_amount = (g_cargo_amount + 0x31) / 0x32;
                        g_cur_nation_rec->muskets = g_cur_nation_rec->muskets + g_cargo_amount;
                        unload_cargo(u, 0);
                    } else if (goods == 8) {
                        unload_cargo(u, 0);
                        g_cur_nation_rec->horses += g_cargo_amount;
                    } else
                        add_gold(g_cur_nation,
                                                  (long)cargo_to_europe(u, 0, 0x64));
                }
                if (loaded != 0 &&
                    g_unit_types[g_units[u].type].holds == (unsigned char)cap) {
                    loaded = 0;
                    again = 1;
                    set_course_for_new_world(u);
                    continue;
                }
                for (goods = 0xf; goods >= 0 &&
                     (g_unit_types[g_units[u].type].holds - g_units[u].cargo_count) != 0 &&
                     crowded == 0 && g_nation_colony_count[nation] != 0; goods--) {
                    if (g_goods_wanted[goods] >= (signed char)want || skip != 0) {
                        if ((bought == 0 && bought_tools == 0) ||
                            (g_unit_types[g_units[u].type].holds - g_units[u].cargo_count) > 2 ||
                            skip != 0) {
                            cost = europe_ask_price(goods) * g_cargo_amount;
                            if ((long)cost <= g_cur_nation_rec->gold) {
                                cargo_from_europe(u, goods, 0x64);
                                g_goods_wanted[goods]--;
                            }
                        }
                    }
                }
                set_course_for_new_world(u);
                again = 1;
            }
        }
    } while (again != 0);
out:
    ;
}
