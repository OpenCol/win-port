/* plan_strategy: candidate for 1028:0f9e (8865 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3
 * symbol: plan_strategy
 * name-source: sibling-build
 *
 * ONE COMPUTER NATION'S WHOLE TURN OF THINKING, before a single unit moves. It walks the
 * units, the colonies, the native settlements and the sixteen land regions in that order,
 * writes what it learns into the shared tables, and then walks the units a second time and
 * gives each one an order. Everything the AI does for the rest of the turn is decided here.
 *
 * FIVE PASSES.
 *
 * 1. THE UNITS IT OWNS get their per-turn flags rebuilt: the carried-cargo bits from
 *    `stack_check`, a "fortify here" bit for the first ship that could carry settlers,
 *    and a sketch bit in the coarse 18x15 map at SEG24:0x540 that later passes read as
 *    "somebody of mine is here". A unit standing where it cannot be -- off the map, or a
 *    land unit at sea -- has its state reset. Units of OTHER nations that this nation can see
 *    are written into the target table as threats: any ship (a frigate only after
 *    independence), and only when the diplomatic pair-flags say so or the ship is a privateer.
 *
 * 2. THE COLONIES. A foreign colony is scored as a target: the fleet strength both sides have
 *    in its region decides whether it is worth attacking at all, and a landing square is
 *    picked by counting how many sea squares adjacent to the colony are themselves adjacent to
 *    open water -- the best beachhead, not the nearest square. A colony of this nation instead
 *    becomes a CARGO entry: its stock of each of the sixteen goods, valued at that nation's own
 *    price table and clamped to 0x7fff, goes into the sorted 16-entry table through
 *    `add_pickup`, so the trade AI can pick the richest ports first. Food, lumber and
 *    trade goods are skipped; tools and muskets only count when the colony's changed-goods word says
 *    they moved, and then a hundred is taken off first.
 *
 * 3. FIVE CONSECUTIVE SWEEPS then release the colony's garrison, in a fixed order of who is
 *    expendable: artillery first, then soldiers whose spec is not JOB_SOLDIER, then veteran soldiers,
 *    then the same two kinds of dragoons. Each released unit gets mission 0x41 and the colony's garrison
 *    count drops by one.
 *
 * 4. THE NATIVE SETTLEMENTS add their size (Settlement +4) into a per-tribe, per-region table held on the
 *    stack, and a settlement in a region where this nation has no presence yet gets a landing
 *    square of its own -- the same beachhead search, and the region is then marked claimed so
 *    two expeditions are never sent to the same place.
 *
 * 5. THE REGIONS are classified last. Each of the sixteen gets a state byte at SEG24:0x500:
 *    6 for "big enough to be worth holding", 0 for "too crowded", 4 for "contested", 3 for
 *    "outmatched", and 4 again when this nation has nothing there at all. The comparison is
 *    made against every other European power and every tribe, and the tie-breaks are the
 *    strength tables at SEG20:0xdab0 and 0xdb30.
 *
 * THE SECOND UNIT PASS then scores every one of the 0x40 target cells for every unit that is
 * free to move: distance times the cell's running weight, divided by the cell's priority plus
 * one, and the cheapest wins. A cell is only eligible if the unit's type bitmask at +0xd of its
 * 14-byte type record allows that kind of cell, if it is in the same region (or the unit is a
 * ship), and if the unit is carrying what the cell needs. Taking a cell bumps that cell's
 * weight, which is what stops every unit in the nation from being sent to the same square.
 * The weights all start at `g_nation_unit_count[nation] >> 3` clamped to 3..0x63, so a nation that is
 * doing well spreads out more.
 *
 * WHAT THE BYTES SETTLED. `ABS` is a source-level MACRO and not `abs()`: the shipped code
 * computes the expression twice and negates with `NOT/INC`, where the library `abs` is
 * `CWD/XOR/SUB` and evaluates once. Every nation comparison against the parameter is a BYTE
 * compare (`CMP AL,[BP+6]`), so the source casts both sides to `char` -- promoting either side
 * to `int` adds a `MOV AH,0` that the shipped does not have. `continent_at` and
 * `site_get` are declared here as returning a BYTE, because the shipped widens their
 * result with `MOV AH,0` instead of using AX as it stands. And the garrison tests are written
 * `(int)colony->want_defenders > 0`, because the cast is what turns `CMP BYTE PTR,0` into the
 * `CBW / OR AX,AX` the shipped uses.
 *
 * Rule 12 decided eleven separate guards. Where the shipped jumps with a LONG conditional the
 * source is a structured `if` around the rest of the body; where it jumps short over a `JMP`
 * the source is a `continue`. Both spellings are the same C and differ by three bytes each,
 * and getting one wrong moves every later jump.
 *
 * RENAMED 2026-09-26 from ai_survey_and_assign: the Mac build's plan_strategy(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (callee add_pickup; callee add_plan; callee clear_pickup; callee minimax; callee on_map), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "nations.h"
#include "jobs.h"
#include "control.h"
#include "aicol.h"
#include "building.h"
#include "gameflag.h"
#include "tribes.h"
#include "units.h"
#define ABS(v) ((v) > 0 ? (v) : ~(v) + 1)
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_europe(int n);

extern int stack_check(int u, int kind);
extern int top_unit(int u);
extern int next_unit(int u);

extern int is_colonist(int u);
extern int terrain_is_ocean(int x, int y);
extern int get_treaty(int nation_a, int nation_b);
extern void set_colony(int colony);
extern int land_region_at(int x, int y);
extern unsigned char site_get(int x, int y);
extern int top_unit_in_square(int x, int y);
extern int has_building(int bit);
extern unsigned char continent_at(int x, int y);
extern int is_unit(int x, int y);
extern int get_warehouse(void);
extern void set_village(int s);
extern int pissed(int tribe, int nation);
extern void set_tribe(int tribe);
extern int map_dist(int x, int y, int tx, int ty);
extern int is_colony(int x, int y);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int menaced(int x, int y, int nation, int flag);
extern void add_plan(int nation, int x, int y, int kind, int priority);
extern void add_pickup(int colony, int value, int loads, int troops);

extern void clear_pickup(void);

void plan_strategy(int nation)
{
    register int nx;
    int i;
    int u;
    int best_slot;
    int head;
    int best;
    int scale;
    int region;
    int x;
    int y;
    int dir;
    int d2;
    int dx;
    int dy;
    int ny;
    int bestx;
    int besty;
    int score;
    int n;
    int sum1;
    int sum2;
    int cnt_a;
    int cnt_b;
    int prev;
    int loads;
    int flag;
    int goods;
    int cap;
    int amt;
    int good_loads;
    int tval;
    int acc;
    int tally;
    int flag1;
    int cnt;
    int ok;
    int hostile;
    int found;
    int has_troops;
    int saveu;
    int def;
    int f54;
    int f56;
    int has_army;
    int has_pioneers;
    long value;
    int t1;
    int t2;
    int tribe_size[8][16];
    int weight[64];

    set_europe(nation);
    memset_rtl(g_sketch, 0, 0x10e);
    memset_rtl(g_region_unit_tally, 0, 0x10);
    memset_rtl(g_region_pop, 0, 0x10);
    memset_rtl(tribe_size, 0, 0x100);
    scale = minimax(g_nation_unit_count[nation] >> 3, 3, 0x63);
    for (i = 0; i < 0x40; i++)
        weight[i] = scale;
    found = 0;
    for (u = 0; g_game.unit_count > u; u++) {
        if ((char)(g_units[u].flags & 0xf) == (char)nation) {
            x = g_units[u].x;
            y = g_units[u].y;
            if ((int)g_units[u].mission == 0x41)
                g_units[u].mission = 0x47;
            g_units[u].flags2 &= 0xd1;
            if (g_units[u].orders == ORDERS_FORTIFY || g_units[u].orders == ORDERS_FORTIFIED)
                g_units[u].flags2 |= 2;
            has_army = (stack_check(u, 4) > 1
                       || stack_check(u, 6) != 0);
            has_pioneers = stack_check(u, 3);
            if (has_army != 0 || has_pioneers != 0) {
                ok = 1;
                if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
                    if (g_unit_types[g_units[u].type].holds - g_units[u].cargo_count != 0)
                        ok = 0;
                    if (ok != 0) {
                        saveu = u;
                        for (u = top_unit(u); ok != 0 && u >= 0;
                             u = next_unit(u)) {
                            if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
                                continue;
                            if (g_unit_types[g_units[u].type].holds - g_units[u].cargo_count == 0)
                                continue;
                            if (u < saveu)
                                ok = 0;
                        }
                        u = saveu;
                    }
                }
                if (ok != 0) {
                    if (has_army != 0)
                        g_units[u].flags2 |= 0xc;
                    if (has_pioneers != 0)
                        g_units[u].flags2 |= 4;
                }
            }
            if (found == 0 && g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST
                && !(g_units[u].flags2 & 0xc)) {
                if (g_unit_tally[nation][UNIT_MERCHANTMAN] + g_unit_tally[nation][UNIT_GALLEON] < 2
                    || g_unit_tally[nation][UNIT_MERCHANTMAN] == 0) {
                    if (g_units[u].type == UNIT_CARAVEL && g_unit_tally[nation][UNIT_CARAVEL] > 1) {
                        g_units[u].flags2 |= 0x20;
                        found = 1;
                    }
                } else {
                    if (g_units[u].type == UNIT_MERCHANTMAN) {
                        g_units[u].flags2 |= 0x20;
                        found = 1;
                    }
                }
            }
            if (on_map(x, y) == 0) {
                g_units[u].orders = ORDERS_SENTRY;
                continue;
            }
            g_sketch[x >> 2][y >> 2] |= is_colonist(u) ? 5 : 1;
            if (g_units[u].orders == 3 || g_units[u].orders == ORDERS_TRADE_ROUTE
                || g_units[u].orders == ORDERS_SENTRY
                || (g_units[u].orders >= 0xa && (int)g_units[u].mission != 0x31))
                g_units[u].orders = ORDERS_NONE;
            if (menaced(x, y, nation, 1) >= 0)
                g_units[u].orders = 0xa;
            if (terrain_is_ocean(x, y) != 0
                && (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST))
                g_units[u].orders = ORDERS_SENTRY;
        } else if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
            if (g_units[u].type != UNIT_FRIGATE || (g_game.game_flags & GAMEFLAG_INDEPENDENCE)) {
                if ((g_units[u].flags & UNIT_SEEN_BY(nation)) != 0) {
                    if ((get_treaty(nation, (unsigned char)(g_units[u].flags & 0xf)) & 0x60)
                            == 0x20
                        || g_units[u].type == UNIT_PRIVATEER)
                        add_plan(nation, g_units[u].x, g_units[u].y,
                                                      0, 3);
                }
            }
        }
    }
    clear_pickup();
    g_land_claim = 0;
    g_sea_claim = 0;
    for (i = 0; g_game.colony_count > i; i++) {
        set_colony(i);
        if ((char)g_cur_colony_rec->nation != (char)nation) {
            region = land_region_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
            if (g_game.difficulty * g_game.turn_counter > 0xb4 || nation >= 4
                || (site_get(g_cur_colony_rec->x, g_cur_colony_rec->y)
                    & (0x10 << nation)) != 0
                || g_cur_colony_rec->nation >= NATION_EUROPEAN_COUNT
                || g_nation_text[g_cur_colony_rec->nation].control != CONTROL_HUMAN) {
                hostile = ((get_treaty(nation, g_cur_colony_rec->nation) & 0x48)
                           == 0x40);
                if (g_region_colony_count[nation][region] + g_region_unit_count[nation][region] != 0
                    && ((g_game.turn_counter + i) & 3) != 0) {
                    u = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y);
                    def = stack_check(u, 2) + g_cur_colony_rec->pop;
                    if (6 - g_game.turn_counter / 0x32 < def)
                        add_plan(nation, g_cur_colony_rec->x,
                                                      g_cur_colony_rec->y, 4,
                                                      hostile ? 3 : 5);
                }
                if (hostile != 0)
                    continue;
                u = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y);
                if ((g_cur_colony_rec->status & 0x40)
                    && ((g_game.turn_counter + u) % 4 != 0
                        || stack_check(u, 0xd) != 0)) {
                    best = 0;
                    flag1 = has_building(BUILDING_FORT);
                    for (dy = -2; dy <= 2; dy++) {
                        for (dx = -2; dx <= 2; dx++) {
                            if (dx == 0 && dy == 0)
                                continue;
                            if (ABS(dx) != 2 && ABS(dy) != 2)
                                continue;
                            x = g_cur_colony_rec->x + dx;
                            y = g_cur_colony_rec->y + dy;
                            if (on_map(x, y) == 0)
                                continue;
                            if (terrain_is_ocean(x, y) == 0)
                                continue;
                            if (continent_at(x, y) != 1)
                                continue;
                            cnt = 0;
                            for (dir = 0; dir < 8; dir++) {
                                nx = g_dir_dx[dir] + x;
                                ny = g_dir_dy[dir] + y;
                                if (on_map(nx, ny) == 0)
                                    continue;
                                if (terrain_is_ocean(nx, ny) == 0)
                                    continue;
                                if (continent_at(nx, ny) != 1)
                                    continue;
                                if (ABS(nx - g_cur_colony_rec->x) > 1)
                                    continue;
                                if (ABS(ny - g_cur_colony_rec->y) > 1)
                                    continue;
                                cnt++;
                            }
                            if (cnt > best) {
                                best = cnt;
                                bestx = x;
                                besty = y;
                            }
                        }
                    }
                    if (best > 0)
                        add_plan(nation, bestx, besty, 0,
                            *min_ptr_1028_e5cd((t2 = 2, &t2),
                                (t1 = (g_cur_colony_rec->pop + 4) >> 3, &t1)) + 2);
                }
            }
            if (g_game.difficulty * g_game.turn_counter > 0xc8 || nation >= 4
                || (site_get(g_cur_colony_rec->x, g_cur_colony_rec->y)
                    & (0x10 << nation)) != 0) {
                f54 = 0;
                f56 = 0;
                if (g_region_colony_count[nation][region]
                        < g_region_colony_count[g_cur_colony_rec->nation][region]
                    && g_region_population[g_cur_colony_rec->nation][region] >= 8)
                    f54 = 1;
                if (g_region_colony_count[nation][region] == 0
                    && g_region_population[g_cur_colony_rec->nation][region] < 8)
                    f56 = 1;
                if (f54 != 0 || f56 != 0) {
                    best = -99;
                    bestx = g_cur_colony_rec->x;
                    besty = g_cur_colony_rec->y;
                    for (dx = -3; dx <= 3; dx++) {
                        for (dy = -3; dy <= 3; dy++) {
                            x = g_cur_colony_rec->x + dx;
                            y = g_cur_colony_rec->y + dy;
                            if (terrain_is_ocean(x, y) == 0 || continent_at(x, y) != 1)
                                continue;
                            ok = 0;
                            for (d2 = 0; d2 < 8; d2++) {
                                nx = g_dir_dx[d2] + x;
                                ny = g_dir_dy[d2] + y;
                                if (terrain_is_ocean(nx, ny) != 0)
                                    continue;
                                if (land_region_at(nx, ny) != region)
                                    continue;
                                ok++;
                            }
                            if (ok == 0)
                                continue;
                            cnt = (ABS(dx) + ABS(dy) + ok) * 2;
                            if (cnt >= best) {
                                best = cnt;
                                bestx = x;
                                besty = y;
                            }
                        }
                    }
                    if (best > 0
                        && is_unit(bestx, besty) < 0) {
                        if (f54 != 0)
                            g_land_claim |= 1 << region;
                        else
                            g_sea_claim |= 1 << region;
                        cnt = 2;
                        if (f54 != 0)
                            cnt++;
                        if (g_cur_colony_rec->nation < NATION_EUROPEAN_COUNT
                            && g_nation_text[g_cur_colony_rec->nation].control == CONTROL_HUMAN) {
                            cnt++;
                            if (g_region_colony_count[0][region] + g_region_colony_count[1][region]
                                    + g_region_colony_count[2][region] + g_region_colony_count[3][region]
                                == g_region_colony_count[g_cur_colony_rec->nation][region]) {
                                if (g_region_size[region] >= 0x10)
                                    cnt++;
                                if (g_region_size[region] >= 0x40)
                                    cnt++;
                            }
                        }
                        if ((g_region_colony_count[0][region] + g_region_colony_count[1][region]
                             + g_region_colony_count[2][region] + g_region_colony_count[3][region]) << 4
                            > g_region_size[region])
                            cnt--;
                        if ((get_treaty(nation, g_cur_colony_rec->nation) & 0x60)
                            == 0x20)
                            cnt++;
                        if (g_game.turn_counter < 0x96)
                            cnt = cnt * 2;
                        u = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y);
                        def = stack_check(u, 2) + g_cur_colony_rec->pop;
                        if (6 - g_game.turn_counter / 0x32 >= def) {
                            f54 = 0;
                            f56 = 0;
                        }
                        if (f54 != 0 || f56 != 0)
                            add_plan(nation, bestx, besty,
                                                          f54 ? 7 : 1, cnt);
                    }
                }
            }
        } else {
            g_sketch[g_cur_colony_rec->x >> 2][g_cur_colony_rec->y >> 2] |= 2;
            if (!(g_colonies[i].status & 0x40))
                continue;
            if (g_cur_colony_rec->ai_flags & (COLONY_AI_BESIEGED | COLONY_AI_BLOCKADED))
                add_plan(nation, g_cur_colony_rec->x,
                                              g_cur_colony_rec->y, 0,
                                              (g_cur_colony_rec->ai_flags & COLONY_AI_BLOCKADED) ? 8 : 5);
            value = 0L;
            loads = 0;
            has_troops = 0;
            flag = 0;
            cap = get_warehouse();
            region = land_region_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
            head = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y);
            for (u = head; u >= 0; u = next_unit(u)) {
                if (g_units[u].type == UNIT_PIONEERS && !(g_cur_colony_rec->ai_flags & COLONY_AI_LAND_UNIMPROVED)) {
                    loads++;
                    flag = 1;
                    value += 0x320;
                }
                if (g_region_state[nation][region] == 0
                    && (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
                    && g_unit_types[g_units[u].type].attack > 1
                    && (int)g_units[u].mission != 0x47
                    && (int)g_units[u].mission != 0x41) {
                    has_troops = 1;
                    flag = 1;
                    value += 0x5dc;
                    loads++;
                }
            }
            for (goods = 0; goods < 0x10; goods++) {
                amt = g_cur_colony_rec->stock[goods];
                if (amt >= cap)
                    amt = amt * 2;
                else if (goods == 8) {
                    amt = 0x19 - (cap - amt);
                    amt = max_int_1028_e350(0, amt);
                }
                good_loads = (min_int_1028_e376(amt, cap) + 0x19) / 100;
                if (goods == 0)
                    continue;
                if (goods == 5)
                    continue;
                if (goods == 0xd)
                    continue;
                if (goods == 0xe || goods == 0xf) {
                    if (!(g_cur_colony_rec->changed & (1 << goods)))
                        continue;
                    amt -= 0x64;
                }
                if (amt >= 0x4b)
                    flag = 1;
                if (amt >= 0) {
                    value += (long)amt * g_goods_value[nation][goods];
                    loads += good_loads;
                }
            }
            if (flag != 0) {
                g_colony_tally[nation]++;
                value += g_cur_colony_rec->turns_since_ship * 8;
                value = min_long_1028_e39c(0x7fffL, value);
                add_pickup(i, (int)value, loads, has_troops);
            }
            if ((int)g_cur_colony_rec->want_defenders > 0) {
                tally = stack_check(head, 0xa);
                if (g_cur_colony_rec->want_defenders > tally)
                    add_plan(nation, g_cur_colony_rec->x,
                        g_cur_colony_rec->y, 3,
                        (char)g_cur_colony_rec->nation == (char)nation
                            ? g_cur_colony_rec->want_defenders - tally + 2 : 2);
                for (u = head; u >= 0 && (int)g_cur_colony_rec->want_defenders > 0;
                     u = next_unit(u)) {
                    if (g_units[u].type == UNIT_ARTILLERY) {
                        g_cur_colony_rec->want_defenders--;
                        if (g_cur_colony_rec->want_defenders_threat != 0)
                            g_cur_colony_rec->want_defenders_threat--;
                        g_units[u].mission = 0x41;
                    }
                }
                for (u = head; u >= 0 && (int)g_cur_colony_rec->want_defenders > 0;
                     u = next_unit(u)) {
                    if (g_units[u].type == UNIT_SOLDIERS && (int)g_units[u].spec != JOB_SOLDIER) {
                        g_cur_colony_rec->want_defenders--;
                        if (g_cur_colony_rec->want_defenders_threat != 0)
                            g_cur_colony_rec->want_defenders_threat--;
                        g_units[u].mission = 0x41;
                    }
                }
                for (u = head; u >= 0 && (int)g_cur_colony_rec->want_defenders > 0;
                     u = next_unit(u)) {
                    if (g_units[u].type == UNIT_SOLDIERS && (int)g_units[u].spec == JOB_SOLDIER) {
                        g_cur_colony_rec->want_defenders--;
                        if (g_cur_colony_rec->want_defenders_threat != 0)
                            g_cur_colony_rec->want_defenders_threat--;
                        g_units[u].mission = 0x41;
                    }
                }
                for (u = head; u >= 0 && (int)g_cur_colony_rec->want_defenders > 0;
                     u = next_unit(u)) {
                    if (g_units[u].type == UNIT_DRAGOONS && (int)g_units[u].spec != JOB_SOLDIER) {
                        g_cur_colony_rec->want_defenders--;
                        if (g_cur_colony_rec->want_defenders_threat != 0)
                            g_cur_colony_rec->want_defenders_threat--;
                        g_units[u].mission = 0x41;
                    }
                }
                for (u = head; u >= 0 && (int)g_cur_colony_rec->want_defenders > 0;
                     u = next_unit(u)) {
                    if (g_units[u].type == UNIT_DRAGOONS && (int)g_units[u].spec == JOB_SOLDIER) {
                        g_cur_colony_rec->want_defenders--;
                        if (g_cur_colony_rec->want_defenders_threat != 0)
                            g_cur_colony_rec->want_defenders_threat--;
                        g_units[u].mission = 0x41;
                    }
                }
            }
        }
    }
    for (i = 0; g_game.settlement_count > i; i++) {
        set_village(i);
        region = land_region_at(g_cur_settlement_rec->x, g_cur_settlement_rec->y);
        tval = pissed(g_cur_tribe, nation);
        tribe_size[g_cur_tribe][region] += g_cur_settlement_rec->size;
        if (g_region_colony_count[nation][region] + g_region_unit_count[nation][region] != 0) {
            if (tval >= 0x4b || (get_treaty(nation, g_cur_tribe_nation) & 2))
                add_plan(nation, g_cur_settlement_rec->x,
                                              g_cur_settlement_rec->y, 4,
                                              (int)g_cur_settlement_rec->mission >= 0 ? 4 : 2);
        }
        if (g_land_claim & (1 << region))
            continue;
        if (g_sea_claim & (1 << region))
            continue;
        if (g_region_colony_count[nation][region] != 0)
            continue;
        bestx = -1;
        besty = -1;
        best = -1;
        for (dir = 0; dir < 8; dir++) {
            x = g_cur_settlement_rec->x + g_dir_dx[dir];
            y = g_cur_settlement_rec->y + g_dir_dy[dir];
            if (terrain_is_ocean(x, y) != 0 && continent_at(x, y) == 1) {
                cnt = 0;
                for (d2 = 0; d2 < 8; d2++) {
                    nx = g_dir_dx[d2] + x;
                    ny = g_dir_dy[d2] + y;
                    if (terrain_is_ocean(nx, ny) == 0
                        && land_region_at(nx, ny) == region)
                        cnt++;
                }
                if (cnt > best) {
                    best = cnt;
                    bestx = x;
                    besty = y;
                }
            }
        }
        if (bestx > 0) {
            add_plan(nation, bestx, besty, 1, 2);
            g_sea_claim |= 1 << region;
        }
    }
    for (i = 0; i < 0x10; i++) {
        cnt_a = cnt_b = 0;
        sum1 = 0;
        sum2 = 0;
        prev = g_region_state[nation][i];
        for (n = 0; n < 4; n++) {
            sum1 += g_region_population[n][i];
            sum2 += g_region_colony_count[n][i];
            if (nation != n) {
                if (g_region_colony_count[n][i] != 0 || g_region_unit_count[n][i] != 0) {
                    if ((get_treaty(nation, n) & 0x60) == 0x20
                        || (get_treaty(nation, n) & 0x48) != 0x40) {
                        if (g_nation_region[nation][i] > g_nation_region[n][i]
                            || g_region_colony_count[nation][i] == 0)
                            cnt_a++;
                        else
                            cnt_b++;
                    }
                }
            }
        }
        for (n = 4; n < 0xc; n++) {
            set_tribe(n - 4);
            sum1 += tribe_size[g_cur_tribe][i] * 2;
            if (g_tribe_region[g_cur_tribe][i] != 0 || tribe_size[g_cur_tribe][i] != 0) {
                if (pissed(g_cur_tribe, nation) >= TRIBE_ATTITUDE_WAR
                    || (get_treaty(nation, n) & 2)) {
                    if (g_nation_region[nation][i] > g_tribe_region[g_cur_tribe][i]
                        || g_region_colony_count[nation][i] == 0)
                        cnt_a++;
                    else
                        cnt_b++;
                }
            }
        }
        g_region_state[nation][i] =
            (g_region_size[i] < (g_region_colony_count[nation][i] + sum2) * 0x14) ? 0 : 6;
        if (cnt_a != 0)
            g_region_state[nation][i] = 4;
        if (cnt_b != 0)
            g_region_state[nation][i] = 3;
        if (g_region_unit_count[nation][i] == 0 && g_region_colony_count[nation][i] == 0)
            g_region_state[nation][i] = 4;
        if ((char)g_region_state[nation][i] != (char)prev)
            ;
        for (head = 0; g_game.colony_count > head; head++) {
            set_colony(head);
            if ((char)g_cur_colony_rec->nation == (char)nation)
                continue;
            if (land_region_at(g_cur_colony_rec->x, g_cur_colony_rec->y) != i)
                continue;
            g_region_pop[i] = max_int_1028_e350(g_region_pop[i], g_cur_colony_rec->pop);
        }
        acc = 0;
        for (n = 0; n < 4; n++) {
            if (n == nation)
                continue;
            acc += g_region_unit_count[n][i];
        }
        acc = min_int_1028_e376(acc, 4);
        g_region_pop[i] = max_int_1028_e350(g_region_pop[i], acc);
    }
    for (u = 0; g_game.unit_count > u; u++) {
        if ((char)(g_units[u].flags & 0xf) == (char)nation) {
            if ((int)g_units[u].mission == 0x41)
                continue;
            if (g_units[u].orders < 0xa)
                g_units[u].mission = 0x3f;
            if (g_units[u].orders != ORDERS_NONE && g_units[u].orders != ORDERS_FORTIFY && g_units[u].orders != ORDERS_FORTIFIED)
                continue;
            if ((int)g_units[u].mission == 0x74 || (int)g_units[u].mission == 0x69)
                g_units[u].mission = 0x3f;
            best = 0x270f;
            best_slot = -1;
            x = g_units[u].x;
            y = g_units[u].y;
            region = land_region_at(x, y);
            if (g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS) {
                acc = g_region_unit_count[nation][region];
                if (acc < 3) {
                    if (acc < 2)
                        continue;
                    if (g_region_colony_count[nation][region] == 0)
                        continue;
                }
            }
            for (i = 0; i < 0x40; i++) {
                if ((int)g_plans[nation][i].kind != -1) {
                    if ((g_unit_types[g_units[u].type].bits
                         & (1 << g_plans[nation][i].kind)) != 0) {
                        if (land_region_at(g_plans[nation][i].x,
                                                      g_plans[nation][i].y) == region
                            || (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)) {
                            if ((int)g_plans[nation][i].kind == 1
                                && !(g_units[u].flags2 & 4))
                                continue;
                            if ((int)g_plans[nation][i].kind == 7
                                && !(g_units[u].flags2 & 8))
                                continue;
                            score = map_dist(x, y,
                                        g_plans[nation][i].x, g_plans[nation][i].y)
                                    * weight[i] / (g_plans[nation][i].priority + 1);
                            if (g_units[u].orders == ORDERS_FORTIFY || g_units[u].orders == ORDERS_FORTIFIED) {
                                if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST) {
                                    if (is_colony(x, y) >= 0)
                                        continue;
                                    if ((int)g_plans[nation][i].priority <= 2
                                        || (g_plans[nation][i].priority * scale < score
                                            && weight[i] != scale))
                                        continue;
                                }
                            }
                            if (score < best
                                && g_plans[nation][i].priority * 3 >> 1 >= score / scale) {
                                best = score;
                                best_slot = i;
                            }
                        }
                    }
                }
            }
            if (best_slot >= 0) {
                g_units[u].mission = 0x31;
                if ((int)g_plans[nation][best_slot].kind == 1)
                    g_units[u].mission = 0x74;
                else if ((int)g_plans[nation][best_slot].kind == 7)
                    g_units[u].mission = 0x69;
                g_units[u].orders = ORDERS_MISSION;
                g_units[u].dest_x = g_plans[nation][best_slot].x;
                g_units[u].dest_y = g_plans[nation][best_slot].y;
                if ((int)g_plans[nation][best_slot].kind != 4)
                    weight[best_slot]++;
            }
        }
    }
}
