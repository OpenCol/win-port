/* treaty_encounters: candidate for 1030:7609 (2086 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 2086, the census figure; the gap to 1030:7e2f is exactly 2086, so there is no dead
 * second epilogue here.
 *
 * WHAT THE EIGHT SQUARES AROUND A MOVING UNIT DO TO IT. `1020:cfb8` and `1030:84d8` call it with the
 * unit and the square it is stepping onto. It owns SHIPRUN and SHIPSLOW. Four separate things happen
 * per neighbouring square, and only for a square whose nation is not the mover's own.
 *
 *   1. THE BLOCKADE. A ship (types 13..18) moving at sea past a sea square holding foreign units is
 *      slowed by each enemy warship on it -- 4 turns of delay for a type 0x10, 6 for 0x11, 8 for 0x12
 *      -- unless the two nations hold pair flag 0x40 and the mover is not a type 0x10 itself. Whether
 *      the delay sticks is a die roll over the two ships' ranges (`blockade_speed`,
 *      the blockader's plus two): under the mover's own range it slips past clean, exactly equal it
 *      takes half. Each enemy is tried in turn until the mover's accumulated +5 reaches its rating.
 *      A clean pass shows SHIPRUN to either side that is human; a delay adds to +5 and shows SHIPSLOW.
 *   2. THE FORT. Passing a foreign colony square, a ship pays 0x32 turns of delay for a fortress (building
 *      2) or 2 for a fort (building 1), and the human mover is told with SHIPSLOW again.
 *   3. WAKING THE NEIGHBOURS. Units on the square with orders 1 wake up (orders 0) if they are ships
 *      or are not standing on water.
 *   4. FIRST CONTACT, on land only, once per other nation (the 12-entry `seen` table). Two tribes
 *      meeting exchange the byte at Tribe+8, both taking the larger. Otherwise the pair is ordered so
 *      the human European comes first, and either `negotiate_tribe` or, for two Europeans and
 *      while SEG20:0x7784 bit 0 is clear, `negotiate_europe` runs. A contact that took sets
 *      pair flag 0x20 both ways.
 *
 * WHAT THE BYTES SETTLED: both unit walks are `for` loops whose increment is the chain step, because
 * the ship-type test leaves through a short JCC over a long JMP to that increment -- a `continue`
 * (rule 12), not a structured `if`. The SHIPRUN block sits INSIDE the `bonus != 0` test, not beside
 * it: one conditional jump covers the die roll and the message together. The mover's +5 is
 * compared against `(unsigned char)rating` -- the shipped compare is a byte and unsigned, while the
 * local itself is an int the `unsigned char` return was promoted into. `swap` takes
 * `void *` here (SS is pushed before the offset is computed, rule 56), and `occupant` and `u` are the
 * two `register` locals, SI and DI in declaration order (rule 53) -- `u` is written first even so.
 * The function returns nothing.
 *
 * RENAMED 2026-09-26 from ship_evades_blockade: the Mac build's treaty_encounters(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller indian_move; caller move_unit; callee has_building; callee negotiate_europe; callee next_unit), the runner-up gets 5, and the arity agrees (3).
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "building.h"
#include "gameflag.h"
#include "units.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameState g_game;   /* SEG20:0x7782 */

extern unsigned char move_rate(int u);
extern int terrain_is_ocean(int x, int y);
extern int is_city(int x, int y);
extern int is_colony(int x, int y);
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

extern void set_colony(int colony);
extern int has_building(int bit);

extern void popup_say(int slot, int word);
extern int popm(char *msg, int n);
extern void swap(int *a, int *b);
extern int get_treaty(int nation_a, int nation_b);
extern int treaty_on(int nation, int other, int mask);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int blockade_speed(int u);
extern int negotiate_tribe(int a, int b, int u, int dir);
extern int negotiate_europe(int nation, int their_nation, int u, int dir, int flag);

void treaty_encounters(int unit, int x, int y)
{
    int nation;
    int other;
    int dir;
    int nx;
    int ny;
    int a;
    int b;
    int i;
    int at_sea;
    int has_owner;
    int sq;
    int saved;
    int colony;
    int rating;
    int bonus;
    int reach;
    int speed;
    int total;
    int roll;
    int is_tribe;
    int ia;
    int ib;
    char ch;
    int seen[12];
    register int occupant;
    register int u;

    u = unit;
    for (i = 0; i < 12; i++)
        seen[i] = 0;
    occupant = u;
    nation = NATION(g_units[occupant]);
    rating = move_rate(occupant);
    at_sea = terrain_is_ocean(x, y);
    has_owner = is_colony(x, y) >= 0;
    for (dir = 0; dir < 8; dir++) {
        nx = g_dir_dx[dir] + x;
        ny = g_dir_dy[dir] + y;
        sq = is_city(nx, ny);
        occupant = top_unit_in_square(nx, ny);
        if (occupant >= 0)
            other = NATION(g_units[occupant]);
        else
            other = sq;
        a = nation;
        b = other;
        if (b >= 0 && a != b) {
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST && at_sea != 0
                && terrain_is_ocean(nx, ny) != 0 && occupant >= 0
                && ((get_treaty(a, b) & 0x40) == 0
                    || g_units[u].type == UNIT_PRIVATEER)) {
                saved = occupant;
                for (; occupant >= 0 && g_units[u].moves_used < (unsigned char)rating; occupant = next_unit(occupant)) {
                    if (g_units[occupant].type < UNIT_SHIP_FIRST || g_units[occupant].type > UNIT_SHIP_LAST)
                        continue;
                    {
                        bonus = 0;
                        if (g_units[occupant].type == UNIT_PRIVATEER)
                            bonus = 4;
                        if (g_units[occupant].type == UNIT_FRIGATE)
                            bonus = 6;
                        if (g_units[occupant].type == UNIT_MAN_O_WAR)
                            bonus = 8;
                        if (bonus != 0) {
                            speed = blockade_speed(u);
                            reach = blockade_speed(occupant) + 2;
                            total = speed + reach;
                            roll = rand_range(1, total);
                            if (roll < speed)
                                bonus = 0;
                            else if (roll == speed)
                                bonus = bonus >> 1;
                            if (bonus == 0) {
                                if ((a < NATION_EUROPEAN_COUNT && g_nation_text[a].control == CONTROL_HUMAN)
                                    || (b < NATION_EUROPEAN_COUNT && g_nation_text[b].control == CONTROL_HUMAN)) {
                                    popup_say(0, g_unit_types[g_units[u].type].name);
                                    popup_say(1, get_adjective(b));
                                    popup_say(2, get_adjective(a));
                                    popup_say(3, g_unit_types[g_units[occupant].type].name);
                                    popm("SHIPRUN", 0);
                                }
                            }
                        }
                        if (bonus != 0) {
                            g_units[u].moves_used += bonus;
                            popup_say(0, g_unit_types[g_units[u].type].name);
                            popup_say(1, get_adjective(other));
                            popup_say(2, g_unit_types[g_units[occupant].type].name);
                            if (a < NATION_EUROPEAN_COUNT && g_nation_text[a].control == CONTROL_HUMAN)
                                popm("SHIPSLOW", 0);
                        }
                    }
                }
                occupant = saved;
            }
            sq = sq >= 0;
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST && sq != 0
                && g_units[u].moves_used < (unsigned char)rating
                && ((get_treaty(a, b) & 0x40) == 0
                    || g_units[u].type == UNIT_PRIVATEER)) {
                colony = find_colony(nx, ny);
                if (colony >= 0) {
                    set_colony(colony);
                    bonus = -1;
                    if (has_building(BUILDING_FORTRESS)) {
                        g_units[u].moves_used += 0x32;
                        bonus = g_buildings[BUILDING_FORTRESS].name;
                    } else if (has_building(BUILDING_FORT)) {
                        g_units[u].moves_used += 2;
                        bonus = g_buildings[BUILDING_FORT].name;
                    }
                    if (bonus >= 0) {
                        popup_say(0, g_unit_types[g_units[u].type].name);
                        popup_say(1, get_adjective(other));
                        popup_say(2, bonus);
                        if (a < NATION_EUROPEAN_COUNT && g_nation_text[a].control == CONTROL_HUMAN)
                            popm("SHIPSLOW", 0);
                    }
                }
            }
            if (has_owner != 0 || sq != 0 || terrain_is_ocean(nx, ny) == at_sea) {
                if (occupant >= 0) {
                    saved = occupant;
                    for (; occupant >= 0; occupant = next_unit(occupant)) {
                        if (g_units[occupant].orders == ORDERS_SENTRY) {
                            if ((g_units[occupant].type >= UNIT_SHIP_FIRST && g_units[occupant].type <= UNIT_SHIP_LAST)
                                || terrain_is_ocean(g_units[occupant].x, g_units[occupant].y) == 0)
                                g_units[occupant].orders = ORDERS_NONE;
                        }
                    }
                    occupant = saved;
                }
            }
        }
        if (at_sea != 0)
            continue;
        if (terrain_is_ocean(nx, ny) != 0)
            continue;
        if (other >= 0 && nation != other && seen[other] == 0) {
            if (occupant >= 0)
                g_units[occupant].last_dir = dir;
            if (nation >= NATION_EUROPEAN_COUNT && other >= NATION_EUROPEAN_COUNT) {
                ia = nation - 4;
                ib = other - 4;
                ch = *max_char_ptr(&g_tribes[ia].horse_herds, &g_tribes[ib].horse_herds);
                g_tribes[ia].horse_herds = ch;
                g_tribes[ib].horse_herds = ch;
                seen[other] = 1;
            } else {
                if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN)
                    ;
                else if (other < NATION_EUROPEAN_COUNT && g_nation_text[other].control == CONTROL_HUMAN)
                    swap(&a, &b);
                else if (nation >= NATION_EUROPEAN_COUNT && other < NATION_EUROPEAN_COUNT)
                    swap(&a, &b);
                is_tribe = b >= NATION_EUROPEAN_COUNT;
                if (is_tribe != 0)
                    seen[other] = negotiate_tribe(a, b, u, dir);
                else if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0)
                    seen[other] = negotiate_europe(a, b, u, dir, 0);
                if (seen[other] != 0)
                    treaty_on(nation, other, 0x20);
            }
        }
    }
}
