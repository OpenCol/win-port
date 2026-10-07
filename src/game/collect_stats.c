/* collect_stats: candidate for 1030:1cf3 (2622 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * EVERY NUMBER THE GAME KEEPS ABOUT ONE NATION, recounted from the units and the colonies. Twelve
 * counters at DGROUP:0x4b44..0x4b74, a per-unit-type histogram of nineteen entries at SEG20:0xd9de (`g_unit_tally`),
 * and six more tables of sixteen indexed by LAND REGION are zeroed and then rebuilt by two full
 * sweeps. `end_of_game_check` calls it twice a year, once for the King and once for the player, and
 * two other callers use it mid-turn.
 *
 * THE UNIT SWEEP walks every unit in the game. A unit of another nation only contributes a flag: if it
 * is one of the four European powers it marks its land region as contested. A unit of THIS nation is
 * counted into the overall total, its region's total, its type's histogram slot, the colonist count if
 * it is one, and -- for a ship (types 0xd..0x12) -- the cargo-capacity total, the ship count and the
 * armed-ship count. Land units add their defence (`get_combat(u, 0)`) into a land total and their attack
 * (`get_combat(u, 1)`) into `g_military_power`.
 *
 * THREE SENTINEL COORDINATES appear as x values: nation-12, nation-16 and nation-20. Units off the map
 * are parked at an x that encodes both the state and the nation, and the first two count toward
 * DGROUP:0x4b70 while the third counts toward 0x4b74. The comparison is done in a single BYTE, which
 * is why it survives an x that is nominally unsigned.
 *
 * IT ALSO REPAIRS DATA. A unit whose home-colony byte at +6 is negative gets it recomputed from
 * `find_colony` on the spot -- a recount pass that quietly fixes a broken back-pointer.
 *
 * THE COLONY SWEEP counts colonies and sizes, and then does the expensive part: for each colony it
 * clears the two low bits of +0x1b and scans the ELEVEN BY ELEVEN box around it for enemy armed
 * ships (types 0xd..0x12 with a non-zero attack), asking `move_dist` whether each one is within five steps. A unit of type 0x11
 * sets bit 1 and anything else sets bit 0 -- one ternary, one OR -- and the two bits then feed four
 * globals at DGROUP:0x4c0b..0x4c11 that count besieged colonies and their population. The box is
 * 121 squares wide for a five-step test, so most of the scan can never qualify.
 *
 * FINALLY the average colony size at SEG20:0xda48 is divided by the colony count, the sixteen land
 * regions are checked for those with a SEG20:0xda50 figure of 8 or more but no colony of this nation,
 * each one counted into DGROUP:0x4b78, and the saved
 * `g_cur_colony_rec` is put back -- the function is careful not to disturb the caller's current colony
 * even though it calls `set_colony` a hundred times.
 *
 * TWO THINGS WORTH KNOWING. The region-flag array at SEG20:0xdbf0 is NOT per nation, so it always
 * describes whoever this function was last called for. And DGROUP:0x4b78 is only ever incremented
 * here, never cleared, so its meaning depends on something else zeroing it.
 *
 * WHAT THE BYTES SETTLED: the nation nibble is masked at BYTE width -- `AND AL,0xF / MOV AH,0`, not
 * `MOV AH,0 / AND AX,0xF` -- so every use goes through the corpus's `NATION` macro. The home byte and
 * the two order codes at +7 are compared through an explicit `(int)`, which is what produces `CBW /
 * OR AX,AX` where a byte compare would otherwise do. And the chain walk is a `for` with its step in
 * the header and two `continue`s, not a `while` with the step in the body: the `continue`s jump to the
 * `next_unit` call, which only a `for` puts there.
 *
 * THE THREE PER-REGION TABLES are filled in one block here, and `get_combat`'s
 * second argument is what tells them apart -- 0 is defending, 1 is attacking:
 *
 *   SEG20:0xdb30  g_nation_region          the DEFENCE figure, beside `g_nation_land_strength`
 *   SEG20:0xdb70  g_region_attack          the ATTACK figure, beside `g_military_power`
 *   SEG20:0xdbb0  g_nation_region_strength the ATTACK figure again, but only for units the
 *                 other nations can see -- the `is_city < 0` or non-human-and-not-on-mission
 *                 test above it -- beside `g_nation_strength`
 *
 * All three are 16 bytes per nation and `plus`. `is_enemy` reads the
 * middle one both ways round, `(unit_count[a][i] >> 1) + (attack[a][i] >> 1) < attack[b][i]`.
 *
 * The ATTACK strength of units that are explicitly NOT ships goes to `g_military_power`
 * (SEG20:0xd9d6), which is consistent: that global was called `g_fleet_strength` when this
 * note first flagged the oddity, and the Foreign Affairs report captions it with MISC word 98,
 * "Military Power". The naval figure is a different expression entirely.
 *
 * RENAMED 2026-09-26 from recount_nation_stats: the Mac build's collect_stats(short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller end_of_game_check; caller new_turn_bookkeeping; callee land_region_at; callee next_unit; callee on_map), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "aicol.h"
#include "region.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_colony(int colony);
extern void set_village(int s);
extern int get_unit_region(int u);
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

extern int is_city(int x, int y);
extern int is_colony(int x, int y);
extern int land_region_at(int x, int y);
extern int job_of_unit(int u);
extern int get_combat(int u, int attacking);
extern int move_dist(int tx, int ty, int x, int y, int want, int limit);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void plus(unsigned char *p, int delta);

void collect_stats(int nation)
{
    register int u;
    int i;
    int owner;
    int cls;
    int region;
    int str;
    int cx;
    int cy;
    int dx;
    int dy;
    int steps;
    Colony *saved;

    g_nation_unit_count[nation] = 0;
    g_nation_colony_count[nation] = 0;
    g_nation_free_colonists[nation] = 0;
    g_pop_by_nation[nation] = 0;
    g_nation_population[nation] = 0;
    g_nation_land_strength[nation] = 0;
    g_nation_cargo_capacity[nation] = 0;
    g_nation_ship_count[nation] = 0;
    g_nation_armed_ships[nation] = 0;
    g_military_power[nation] = 0;
    g_nation_strength[nation] = 0;
    saved = g_cur_colony_rec;
    for (i = 0; i < 0x13; i++)
        g_unit_tally[nation][i] = 0;
    g_nation_ships_in_transit[nation] = 0;
    g_europe_land_units[nation] = 0;
    g_avg_colony_size[nation] = 0;
    for (i = 0; i < 16; i++) {
        g_region_flags[i] = 0;
        g_region_unit_count[nation][i] = 0;
        g_region_colony_count[nation][i] = 0;
        g_region_population[nation][i] = 0;
        g_nation_region[nation * 16 + i] = 0;
        g_region_attack[nation][i] = 0;
        g_nation_region_strength[nation][i] = 0;
    }
    for (u = 0; g_game.unit_count > u; u++) {
        owner = NATION(g_units[u]);
        region = get_unit_region(u);
        if (owner == nation) {
            if (g_units[u].type < UNIT_BRAVES)
                plus(&g_unit_tally[nation][g_units[u].type], 1);
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
                if (g_units[u].x == (unsigned char)(nation - 12))
                    g_nation_ships_in_transit[nation]++;
                if (g_units[u].x == (unsigned char)(nation - 16))
                    g_nation_ships_in_transit[nation]++;
                plus(&g_nation_cargo_capacity[nation],
                                          g_unit_types[g_units[u].type].holds);
                plus(&g_nation_ship_count[nation], 1);
                if (g_unit_types[g_units[u].type].attack != 0)
                    g_nation_armed_ships[nation]++;
            } else {
                if (g_units[u].x == (unsigned char)(nation - 20))
                    g_europe_land_units[nation]++;
                if (g_unit_types[g_units[u].type].defence > 1
                    && (g_units[u].orders == ORDERS_FORTIFY || g_units[u].orders == ORDERS_FORTIFIED)
                    && is_colony(g_units[u].x, g_units[u].y) < 0
                    && region >= 0)
                    g_region_flags[region] |= REGION_GARRISONED;
            }
            if ((int)g_units[u].home < 0)
                g_units[u].home = find_colony(g_units[u].x, g_units[u].y);
            g_nation_unit_count[nation]++;
            if (region >= 0)
                g_region_unit_count[nation][region]++;
            if (g_units[u].type == UNIT_COLONISTS)
                g_nation_free_colonists[nation]++;
            cls = job_of_unit(u);
            if (cls >= 0) {
                plus(&g_nation_population[nation], 1);
                if (region >= 0)
                    plus(&g_region_population[nation][region], 1);
            }
            if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST) {
                str = get_combat(u, 0);
                plus(&g_nation_land_strength[nation], str);
                if (region >= 0)
                    plus(&g_nation_region[nation * 16 + region], str);
                str = get_combat(u, 1);
                g_military_power[nation] += str;
                if (region >= 0)
                    plus(&g_region_attack[nation][region], str);
                if (is_city(g_units[u].x, g_units[u].y) < 0
                    || ((nation >= 4 || g_nation_text[nation].control != 0)
                        && (int)g_units[u].mission != 0x41
                        && (int)g_units[u].mission != 0x47)) {
                    plus(&g_nation_strength[nation], str);
                    if (region >= 0)
                        plus(&g_nation_region_strength[nation][region], str);
                }
            }
        } else {
            if (region >= 0 && NATION(g_units[u]) < 4)
                g_region_flags[region] |= REGION_RIVAL_UNITS;
        }
    }
    g_colonies_besieged = 0;
    g_pop_besieged = 0;
    g_colonies_blockaded = 0;
    g_pop_blockaded = 0;
    for (i = 0; i < g_game.colony_count; i++) {
        set_colony(i);
        owner = g_cur_colony_rec->nation;
        region = land_region_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
        if (owner == nation) {
            g_nation_colony_count[nation]++;
            g_avg_colony_size[nation] += g_cur_colony_rec->pop;
            plus(&g_nation_population[nation], g_cur_colony_rec->pop);
            plus(&g_pop_by_nation[nation], g_cur_colony_rec->pop);
            if (region >= 0) {
                g_region_colony_count[nation][region]++;
                plus(&g_region_population[nation][region],
                                          g_cur_colony_rec->pop);
            }
            g_cur_colony_rec->ai_flags &= ~(COLONY_AI_BESIEGED | COLONY_AI_BLOCKADED);
            for (dy = -5; dy <= 5; dy++) {
                for (dx = -5; dx <= 5; dx++) {
                    cx = g_cur_colony_rec->x + dx;
                    cy = g_cur_colony_rec->y + dy;
                    if (!on_map(cx, cy))
                        continue;
                    u = top_unit_in_square(cx, cy);
                    if (u < 0)
                        continue;
                    if (NATION(g_units[u]) == (unsigned char)nation)
                        continue;
                    for (; u >= 0; u = next_unit(u)) {
                        if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
                            continue;
                        if (g_unit_types[g_units[u].type].attack == 0)
                            continue;
                        steps = move_dist(cx, cy, g_cur_colony_rec->x,
                                                          g_cur_colony_rec->y, 1, 8);
                        if (steps >= 0 && steps <= 5)
                            g_cur_colony_rec->ai_flags |= g_units[u].type == UNIT_FRIGATE ? COLONY_AI_BLOCKADED : COLONY_AI_BESIEGED;
                    }
                }
            }
            if (g_cur_colony_rec->ai_flags & COLONY_AI_BLOCKADED) {
                g_colonies_blockaded++;
                g_pop_blockaded += g_cur_colony_rec->pop;
            }
            if (g_cur_colony_rec->ai_flags & COLONY_AI_BESIEGED) {
                g_colonies_besieged++;
                g_pop_besieged += g_cur_colony_rec->pop;
            }
        } else {
            if (region >= 0)
                g_region_flags[region] |= REGION_RIVAL_COLONY;
        }
    }
    for (i = 0; i < g_game.settlement_count; i++) {
        set_village(i);
        region = land_region_at(g_cur_settlement_rec->x,
                                           g_cur_settlement_rec->y);
        if (region >= 0)
            g_region_flags[region] |= REGION_NATIVE_SETTLEMENT;
    }
    for (i = 0; i < 16; i++) {
        if (g_region_farmland[i] >= 8 && g_region_colony_count[nation][i] == 0)
            g_unclaimed_regions++;
    }
    if (g_nation_colony_count[nation] != 0)
        g_avg_colony_size[nation] = g_avg_colony_size[nation] / g_nation_colony_count[nation];
    g_cur_colony_rec = saved;
}
