/* PORT: this file's own declarations of get_defense removed: the header declares it from the definition (W1 step 3). */
/* indian_move: candidate for 1030:84d8 (6075 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -a
 * symbol: indian_move
 * name-source: sibling-build
 * size-source: 6075, not the census 6009. The census stops at the last instruction; after it come
 * an unreachable second epilogue and the TWO thirteen-entry jump tables of this function's two
 * switches, and `indian_unit` starts right after the last of them. `-a` is needed
 * for the word alignment in front of them (rule 81).
 *
 * WHERE ONE BRAVE GOES NEXT, and whether it attacks when it gets there. This is the whole movement
 * brain of a native unit: it scores the eight squares around it plus staying put, picks the best,
 * and returns that direction (8 means "stay"). A unit whose settlement has been destroyed is freed
 * on the spot and -1 comes back.
 *
 * THE PRESSURE READING comes first. Before scoring anything the routine walks the eight squares
 * round its OWN settlement and counts the European units standing on them -- every non-ship unit whose type's attack
 * value is over 1 counts one -- discounted by a quarter of any colony's size; the owner of the LAST square with
 * a positive count is remembered as the one pressing hardest, and the total has to reach two to count at all. It also
 * tallies how many of the four European nations the tribe is already at war with, either by tension
 * over 0x4b or by a settlement grudge over 0x80.
 *
 * EACH CANDIDATE SQUARE then gets a score that starts at 200 and is pushed around by a long list of
 * terms: the terrain's defence value, what is standing there (a treasure train is worth 50 to a
 * lone brave, artillery 35, a loaded wagon 8 more than an empty one), the tension with whoever
 * owns it, the distance back to the settlement, whether a road or a river runs both ways, whether
 * the square continues the direction the unit was already going (+4 straight on, +3 to either side,
 * -6 for turning back), and how close it is to the nearest colony. A wagon train standing NEXT to
 * an empty square is worth raiding through it, which is the `wnat` pass: a neighbour's wagon marks
 * the square as worth taking even though nothing is on it.
 *
 * ATTACKING is decided inside that scoring. A square the tribe can enter but whose owner it is not
 * angry with is dropped unless the roll says otherwise; a square it IS angry about gets the
 * defender looked up through `best_defender`, its strength compared with
 * `get_defense`, and the difference folded into the score -- so a brave walks towards
 * the fight it can win. A colony square is worth 500 outright.
 *
 * AFTERWARDS the chosen direction is written into the unit, the unit either fortifies (staying put
 * sets orders 5, and 6 once the unit already has 5 or 6) or is woken, and a raid on a human colony prints INDIANSURPRISE. A
 * unit that has just been given muskets or horses from the tribe's stock (the tribe record's muskets and horses) is upgraded on
 * the spot. Finally, when nothing was worth attacking and the unit has already moved, the answer is
 * turned back into "stay".
 *
 * WHAT THE BYTES SETTLED: three `if`s have an EMPTY else arm, which is the only thing that explains
 * the `JMP +0` the shipped leaves at the end of their then-blocks (rule 54); `(coldist * 2 + 5) * 2`
 * has to be written with a shift or Borland distributes it into `coldist * 2 + 10`; the unit's
 * direction byte at +0xb and its settlement index at +6 are SIGNED and the map region byte is not;
 * `a = b = c = 0` stores the LAST name first, so the shipped's store order reads the chain
 * backwards; and the two distance calls take `here - there`, not `there - here`, which the operand
 * order of the subtraction gives away.
 *
 * RENAMED 2026-09-26 from tribe_surprise_raid: the Mac build's indian_move(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (callee best_defender; callee get_name; callee popup_say; callee set_colony; callee terrain_at), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

#include "control.h"
#include "dbgopt.h"
#include "terrain.h"
#include "tribes.h"
#include "nations.h"
#include "units.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_tribe(int tribe);
extern unsigned char feature_get(int x, int y);
extern unsigned char map_get(int x, int y);
extern int zoc(int x, int y, int nation);
extern unsigned char continent_at(int x, int y);
extern unsigned char site_get(int x, int y);
extern void spot_unit(int u, int nation);
extern void force_map(int x, int y);
extern int find_close_colony(int x, int y, int nation, int region);
extern void delete_unit(int u);
extern void set_village(int s);
extern int xy_dist(int dx, int dy);
extern int map_dist(int x0, int y0, int x1, int y1);
extern char owner_of(int x, int y);
extern int terrain_is_ocean(int x, int y);
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

extern int pissed(int tribe, int i);

extern int lost_city_at(int x, int y);
extern int is_unit(int x, int y);
extern int is_city(int x, int y);
extern int resource_at(int x, int y);
extern int stack_check(int u, int kind);

extern int piss_factor(int tension);
extern unsigned char move_rate(int u);
extern int bearing_adjacent(int dir, int last_dir);
extern int is_armed(int u);
extern int is_horse(int u);
extern int zoc_4(int x, int y, int nation);
extern void set_colony(int colony);

extern int get_combat(int u, int mode);
extern int top_unit(int u);
extern void end_unit_turn(int u);
extern int possibly_center(int x0, int y0, int x1, int y1, int flag);
extern void popup_say(int slot, int word);
extern int popm(char *name, int n);

extern int move_to(int u);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern int get_treaty(int nation_a, int nation_b);
extern int get_name(int nation);

extern void treaty_encounters(int u, int x, int y);

int indian_move(register int u)
{
    int claim;
    int claim2;
    int ux;
    int uy;
    int nation;
    int tn;
    int wn;
    int sq_nation2;
    int utype;
    int colony;
    int settlement;
    int g4bc6;
    int dist0;
    int hostile_count;
    int hostile_mask;
    int pair;
    int tribe;
    int dir;
    int d2;
    int best;
    int tx;
    int ty;
    int terr;
    int adj_owner;
    int chosen;
    int i;
    int nat0;
    int dist;
    int tension;
    int q;
    int u2;
    int road2;
    int river2;
    int road;
    int river;
    int decoration;
    int penalty;
    int retreat;
    int flags;
    int at_war;
    int angry;
    int sq_nat;
    int shift;
    int def;
    int col2;
    int owner;
    int bonus;
    int unit;
    int civ;
    int valuable;
    int easy_prey;
    int alone;
    int diff;
    int grudge;
    int occupied;
    int defb;
    int best_flags;
    int raid;
    int sx;
    int sy;
    int saveu;
    int target_colony;
    int wnat;
    int ax2;
    int ay2;
    int kind;
    int bestw;
    int wscore;
    int cdist;
    int coldist;
    int colmin;
    int total;
    int worst;
    int cnt;
    int step;
    int turnref;
    int flagged;
    unsigned char region;
    int t1;
    int t2;
    int t3;
    int t4;
    register int score;

    unit = 0;
    best_flags = 0;
    raid = 0;
    flagged = 0;
    turnref = g_game.turn_counter;
    turnref -= turnref / 0x19;
    ux = g_units[u].x;
    uy = g_units[u].y;
    nation = NATION(g_units[u]);
    tribe = nation - 4;
    utype = g_units[u].type;
    set_tribe(tribe);
    road = feature_get(ux, uy) & (MAP1_SETTLEMENT | MAP1_ROAD);
    river = map_get(ux, uy) & MAP0_RIVER;
    claim = zoc(ux, uy, nation);
    claim2 = g_foreign_owner;
    chosen = 8;
    region = continent_at(ux, uy);
    if (flagged == 0 && (g_debug_flags & DBG_INDIAN_AI_MOVEMENT) != 0) {
        if (g_game.reveal_all != 0 || g_game.player_nation >= NATION_EUROPEAN_COUNT ||
            ((0x10 << g_game.player_nation) & site_get(ux, uy)) != 0) {
            spot_unit(u, g_game.player_nation);
            flagged = 1;
        }
    }
    if (flagged != 0)
        force_map(ux, uy);
    colony = find_close_colony(ux, uy, -1, (int)region);
    g4bc6 = g_nearest_dist;
    settlement = (int)g_units[u].home;
    if (settlement < 0 || g_game.settlement_count <= settlement) {
        delete_unit(u);
        chosen = -1;
        goto out;
    }
    set_village(settlement);
    dist0 = xy_dist(ux - g_settlements[settlement].x,
                                      uy - g_settlements[settlement].y);
    if (settlement >= 0 &&
        continent_at(g_settlements[settlement].x, g_settlements[settlement].y) != region)
        settlement = -1;
    coldist = -1;
    if (colony >= 0) {
        coldist = map_dist(g_colonies[colony].x, g_colonies[colony].y,
                                               g_cur_settlement_rec->x,
                                               g_cur_settlement_rec->y);
        colmin = (int)g_colonies[colony].pop >> 1;
        colmin = max_int_1030_ead6(2, colmin);
        if (coldist > colmin)
            coldist = -1;
    }
    saveu = u;
    worst = -1;
    total = 0;
    for (dir = 0; dir < 8; dir++) {
        tx = g_cur_settlement_rec->x + g_dir_dx[dir];
        ty = g_cur_settlement_rec->y + g_dir_dy[dir];
        tn = owner_of(tx, ty);
        if (tn < 0 || tn >= NATION_EUROPEAN_COUNT)
            continue;
        if (terrain_is_ocean(tx, ty) != 0)
            continue;
        u = top_unit_in_square(tx, ty);
        if (u < 0)
            continue;
        cnt = 0;
        for (; u >= 0; u = next_unit(u))
            if ((g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST) &&
                g_unit_types[g_units[u].type].attack > 1)
                cnt++;
        if (cnt > 0) {
            target_colony = find_colony(tx, ty);
            if (target_colony >= 0)
                cnt -= (int)g_colonies[target_colony].pop >> 2;
        }
        if (cnt > 0) {
            total += cnt;
            worst = tn;
        }
    }
    u = saveu;
    if (total < 2) {
        total = 0;
        worst = -1;
    }
    hostile_count = 0;
    hostile_mask = 0;
    for (i = 0; i < 4; i++) {
        if (pissed(tribe, i) >= TRIBE_ATTITUDE_WAR) {
            hostile_count++;
            hostile_mask |= 1 << i;
        }
        if (g_settlements[(int)g_units[u].home].anger[i] >= SETTLEMENT_ANGER_HOSTILE) {
            hostile_count++;
            hostile_mask |= 1 << i;
        }
    }
    best = -1;
    for (dir = 0; dir < 9; dir++) {
        tx = g_dir_dx[dir] + ux;
        ty = g_dir_dy[dir] + uy;
        terr = terrain_at(tx, ty);
        score = 0xc8;
        flags = 0;
        if (terr == TERRAIN_OCEAN || terr == TERRAIN_SEA_LANE)
            continue;
        if (lost_city_at(tx, ty) != 0)
            continue;
        if (terr == TERRAIN_ARCTIC)
            continue;
        tn = owner_of(tx, ty);
        nat0 = is_unit(tx, ty);
        sq_nat = is_city(tx, ty);
        road2 = feature_get(tx, ty) & (MAP1_SETTLEMENT | MAP1_ROAD);
        river2 = map_get(tx, ty) & MAP0_RIVER;
        decoration = (resource_at(tx, ty) != -1);
        if (tn < 0 || tn == nation) {
            at_war = 0;
            angry = 0;
            grudge = 0;
        } else {
            flags |= 1;
            grudge = g_settlements[(int)g_units[u].home].anger[tn];
            if (tn >= NATION_EUROPEAN_COUNT)
                at_war = 0;
            else {
                angry = (grudge >= SETTLEMENT_ANGER_HOSTILE);
                at_war = (pissed(g_cur_tribe, tn) >= TRIBE_ATTITUDE_WAR);
                if (at_war != 0)
                    angry = 1;
                if (tn == worst)
                    angry = 1;
            }
            if (at_war != 0)
                flags |= 4;
            if (angry != 0)
                flags |= 0x40;
        }
        occupied = 0;
        owner = (nat0 >= 0) ? nat0 : sq_nat;
        if (owner == nation)
            owner = -1;
        if (owner >= 0) {
            flags |= 2;
            occupied = 1;
        }
        wnat = -1;
        bestw = -1;
        if (occupied == 0) {
            target_colony = u;
            for (d2 = 0; d2 < 8; d2++) {
                ax2 = g_dir_dx[d2] + tx;
                ay2 = g_dir_dy[d2] + ty;
                if (ax2 == ux && ay2 == uy)
                    continue;
                wscore = 0x64;
                sq_nation2 = is_city(ax2, ay2);
                wn = sq_nation2;
                if (wn < 0) {
                    u = top_unit_in_square(ax2, ay2);
                    if (u < 0)
                        continue;
                    if (NATION(g_units[u]) == (unsigned char)nation)
                        continue;
                    if (g_units[u].type != UNIT_WAGON_TRAIN)
                        continue;
                    wn = NATION(g_units[u]);
                    wscore = wscore >> 1;
                }
                if (wn < 0 || wn >= 4)
                    continue;
                wscore += pissed(g_cur_tribe, wn) >> 1;
                if (wscore >= bestw) {
                    bestw = wscore;
                    wnat = wn;
                    kind = 1;
                    if (sq_nation2 < 0)
                        kind = 2;
                }
            }
            u = target_colony;
        }
        cdist = map_dist(tx, ty, g_cur_settlement_rec->x,
                                             g_cur_settlement_rec->y);
        if (wnat >= 0) {
            grudge = g_settlements[(int)g_units[u].home].anger[wnat];
            angry = (grudge >= SETTLEMENT_ANGER_HOSTILE);
            at_war = (pissed(g_cur_tribe, wnat) >= TRIBE_ATTITUDE_WAR);
            if (angry == 0 && at_war == 0 &&
                turnref - g_units[u].hold.n.turn_stamp < cdist * 2 + 5)
                continue;
            bestw += (turnref - g_units[u].hold.n.turn_stamp) * 2;
            score += bestw;
            if (at_war == 0)
                flags |= 0x80;
        }
        if (occupied == 0 && wnat < 0 && coldist >= 0) {
            if ((g_nation_population[g_colonies[colony].nation] >> 3) + coldist * 2 + 5 <=
                turnref - g_units[u].hold.n.turn_stamp) {
                dist = xy_dist(tx - g_colonies[colony].x,
                                                 ty - g_colonies[colony].y);
                if (dist < 0xc)
                    score = (0xc - dist) * 0xa + score;
            }
        }
        if (occupied != 0) {
            if (tn < NATION_EUROPEAN_COUNT) {
                tension = pissed(tribe, tn);
                tension = min_int_1030_eb20(tension, 0x64);
                if (nat0 >= 0 || sq_nat >= 0) {
                    civ = valuable = easy_prey = 0;
                    alone = (stack_check(u, 2) == 1);
                    if (nat0 >= 0) {
                        u2 = top_unit_in_square(tx, ty);
                        score += g_terrain_rows[terr].defence << 2;
                        for (unit = u2; unit >= 0; unit = next_unit(unit)) {
                            switch (g_units[unit].type) {
                            case UNIT_TREASURE_TRAIN:
                                if (alone != 0) {
                                    score += 0x32;
                                    easy_prey = 1;
                                }
                                civ = 1;
                                break;
                            case UNIT_SOLDIERS:
                            case UNIT_DRAGOONS:
                                if (cdist <= 1)
                                    score += (total << 2) + 0x14;
                                break;
                            case UNIT_ARTILLERY:
                                score = score + 0x23;
                                valuable = 1;
                                easy_prey = alone;
                                break;
                            case UNIT_COLONISTS:
                                score = score + 5;
                                easy_prey = alone;
                                break;
                            case UNIT_PIONEERS:
                                score = score + 0xa;
                                easy_prey = alone;
                                break;
                            case UNIT_SCOUTS:
                                score = score + 0xa;
                                break;
                            case UNIT_WAGON_TRAIN:
                                score = score + 2;
                                if (g_units[unit].cargo_count != 0) {
                                    valuable = 1;
                                    flags |= 0x10;
                                    score = score + 8;
                                }
                                if (valuable != 0 || rand_range(0x32, 0x64) < tension) {
                                    if (g_units[unit].link < 0 && g_units[unit].link2 < 0) {
                                        score = score + 0xa;
                                        valuable = 1;
                                        easy_prey = 1;
                                    }
                                }
                                break;
                            }
                        }
                    }
                    if (sq_nat >= 0) {
                        if (tension >= 0x4b)
                            score += 0x14;
                        if (tension >= 0x32)
                            score += 0xa;
                    }
                    if (tension <= 0x19 && angry == 0) {
                        if (easy_prey == 0 || civ == 0 || rand_range(0, 7) != 0)
                            continue;
                    } else if (sq_nat < 0 && grudge >= 0x20)
                        angry = 1;
                    penalty = 0x32 - tension;
                    score -= penalty;
                    q = 4 - piss_factor(tension);
                    diff = (tn < NATION_EUROPEAN_COUNT && g_nation_text[tn].control == CONTROL_HUMAN) ? g_game.difficulty : 0;
                    if (easy_prey == 0 && angry == 0)
                        continue;
                    flags |= 8;
                    raid = 1;
                } else {
                    penalty = 0x32 - tension;
                    score = score - penalty;
                }
            } else
                continue;
        }
        bonus = 0;
        if (sq_nat == nation) {
            if ((int)g_cur_tribe_rec->muskets > 0 &&
                (g_units[u].type == UNIT_BRAVES || g_units[u].type == UNIT_MOUNTED_BRAVES)) {
                score += 0x14;
                bonus = 1;
            }
            if (g_cur_tribe_rec->horses >= 0x19 && move_rate(u) <= 3) {
                score += 0x14;
                bonus = 1;
            }
        }
        if (dir != 8) {
            if (nat0 >= 0 && nat0 == nation) {
                u2 = top_unit_in_square(tx, ty);
                if (next_unit(u2) >= 0 && sq_nat < 0)
                    continue;
                else
                    score = score - 0x28;
            }
        } else {
            score = score - (stack_check(u, 2) * 0x28 - 0x28);
            if (raid != 0 || rand_range(0, (g_cur_tribe_rec->level + 1) << 2) == 0)
                score -= 0x19;
        }
        if (dir != 8) {
            if ((int)g_units[u].last_dir == dir)
                score += 4;
            else if (bearing_adjacent(dir, (int)g_units[u].last_dir) != 0)
                score += 3;
            else if (((int)g_units[u].last_dir ^ 4) == dir)
                score -= 6;
            if (road2 != 0 && road != 0)
                score += 4;
            else if ((dir & 1) == 0 && river2 != 0 && river != 0)
                score += 4;
        } else {
            if (hostile_count == 0) {
                if (claim != 0) {
                    if (claim2 < 4) {
                        tension = pissed(tribe, claim2);
                        if (piss_factor(tension) > 0)
                            score += ((tension - 0x32) >> 1) + 8;
                    } else
                        ;
                } else if (bonus == 0)
                    continue;
            } else if (claim != 0) {
                if (claim2 < 4 && pissed(tribe, claim2) >= 0x5f)
                    score += 0x10;
                else if (claim2 < 4) {
                    tension = pissed(tribe, claim2);
                    if (piss_factor(tension) > 0)
                        score += ((tension - 0x32) >> 1) + 5;
                } else
                    ;
            }
        }
        if (settlement >= 0) {
            dist = xy_dist(tx - g_settlements[settlement].x,
                                             ty - g_settlements[settlement].y);
            if (dist > 2) {
                retreat = dist * 2 + dist;
                if (hostile_count != 0)
                    retreat = retreat >> 1;
                if (is_armed(u) != 0)
                    retreat = retreat >> 1;
                if (is_horse(u) != 0)
                    retreat = retreat >> 2;
                score -= retreat;
            }
        }
        adj_owner = zoc_4(tx, ty, nation);
        if (adj_owner != 0) {
            if (g_foreign_owner < 4) {
                pair = get_treaty(nation, g_foreign_owner);
                if ((pair & 0x20) == 0)
                    score += 0x32;
                shift = 2;
                tension = pissed(tribe, g_foreign_owner);
                if (piss_factor(tension) > 0)
                    score += (pissed(tribe, g_foreign_owner) - 0x32) >> shift;
            } else
                score -= 0x19;
        }
        if (hostile_count == 0) {
            if (tn < 0)
                score += 5;
            if (colony >= 0 && continent_at(tx, ty) == region) {
                dist = xy_dist(tx - g_colonies[colony].x,
                                                 ty - g_colonies[colony].y);
                tension = pissed(tribe, g_colonies[colony].nation);
                if (dist < 0xc)
                    score = (*max_ptr_1030_f4a6((t2 = 1, &t2),
                             (t1 = piss_factor(tension) + 1, &t1)) *
                             (0xc - dist) >> 2) + score;
            }
        } else {
            if (angry != 0 || at_war != 0) {
                score = score + 5;
                if (decoration != 0)
                    score = score + 0xa;
                col2 = find_colony(tx, ty);
                if (col2 >= 0) {
                    set_colony(col2);
                    score = score + 0x1f4;
                } else if (nat0 >= 0) {
                    def = top_unit_in_square(tx, ty);
                    def = best_defender(def, nation);
                    occupied = get_combat(u, 1) * 3 >> 1;
                    defb = get_defense(def, u);
                    if (g_units[def].type == UNIT_ARTILLERY)
                        defb = defb >> 3;
                    saveu = u;
                    for (u = top_unit(def); u >= 0; u = next_unit(u)) {
                        switch (g_units[u].type) {
                        case UNIT_TREASURE_TRAIN:
                        case UNIT_ARTILLERY:
                        case UNIT_WAGON_TRAIN:
                            score += 0x10;
                            break;
                        case UNIT_PIONEERS:
                        case UNIT_MISSIONARIES:
                        case UNIT_SCOUTS:
                            score += 8;
                            break;
                        case UNIT_COLONISTS:
                            score += 4;
                            break;
                        case UNIT_DRAGOONS:
                            score--;
                            break;
                        case UNIT_SOLDIERS:
                            score -= 2;
                            break;
                        }
                    }
                    u = saveu;
                    if (occupied >= defb)
                        score = score + occupied + 0x1e - defb;
                    else
                        score += (occupied - defb) * 2;
                }
            } else if (sq_nat >= 0)
                continue;
            if (colony >= 0) {
                dist = xy_dist(tx - g_colonies[colony].x,
                                                 ty - g_colonies[colony].y);
                if (continent_at(tx, ty) == region) {
                    pair = get_treaty(nation, g_colonies[colony].nation);
                    if (pissed(tribe, g_colonies[colony].nation) < TRIBE_ATTITUDE_WAR) {
                        tension = pissed(tribe, g_colonies[colony].nation);
                        if (dist < 0xc)
                            score = (*max_ptr_1030_f4a6((t4 = 1, &t4),
                                     (t3 = piss_factor(tension) + 1, &t3)) *
                                     (0xc - dist) >> 2) + score;
                    } else if ((pair & 0x20) != 0)
                        score = score - dist * 2;
                }
            }
        }
        score = rand_range(1, 5) + score;
        score = max_int_1030_ead6(0, score);
        if (flagged != 0)
            ;
        if (score > best) {
            best = score;
            chosen = dir;
            best_flags = flags;
        }
    }
    g_units[u].last_dir = (char)chosen;
    if (chosen == 8) {
        if (g_units[u].orders == ORDERS_FORTIFY || g_units[u].orders == ORDERS_FORTIFIED)
            g_units[u].orders = ORDERS_FORTIFIED;
        else
            g_units[u].orders = ORDERS_FORTIFY;
        if (is_city(ux, uy) == nation) {
            if ((int)g_cur_tribe_rec->muskets > 0 &&
                (g_units[u].type == UNIT_BRAVES || g_units[u].type == UNIT_MOUNTED_BRAVES)) {
                g_units[u].type++;
                if (rand_range(0, g_game.difficulty) == 0)
                    g_cur_tribe_rec->muskets--;
            }
            if (g_cur_tribe_rec->horses >= 0x19 && move_rate(u) <= 3) {
                g_units[u].type = g_units[u].type + 2;
                g_cur_tribe_rec->horses -= 0x19;
            }
        }
    } else
        g_units[u].orders = ORDERS_NONE;
    if ((best_flags & 0xa) != 0) {
        tx = g_dir_dx[chosen] + ux;
        ty = g_dir_dy[chosen] + uy;
        tn = owner_of(tx, ty);
        if (g_cur_tribe_rec->mood[tn] == 2) {
            chosen = 8;
            end_unit_turn(u);
            goto done;
        } else
            g_cur_tribe_rec->mood[tn] = 1;
    }
    angry = best_flags & 0x40;
    at_war = best_flags & 4;
    occupied = best_flags & 2;
    if ((occupied != 0 || (best_flags & 8) != 0) &&
        move_rate(u) - g_units[u].moves_used < 3) {
        chosen = 8;
        end_unit_turn(u);
        goto done;
    }
    if ((best_flags & 8) != 0 && tn < NATION_EUROPEAN_COUNT && g_nation_text[tn].control == CONTROL_HUMAN) {
        colony = find_close_colony(tx, ty, tn, -1);
        if (colony >= 0) {
            possibly_center(tx, ty, tx, ty, 0);
            popup_say(0, get_name(nation));
            set_name_slot(1, g_cur_colony_rec->name);
            popup_say(2, get_adjective(nation));
            popm("INDIANSURPRISE", 1);
        }
    }
    if ((best_flags & 0xa) != 0 || at_war != 0) {
        g_units[u].flags2 &= 0xf7;
        best_flags &= 0xff7f;
    }
    if ((g_units[u].flags2 & 8) != 0) {
        g_units[u].flags2 &= 0xf7;
        treaty_encounters(u, g_units[u].x, g_units[u].y);
        chosen = 8;
        end_unit_turn(u);
        goto done;
    }
    if ((best_flags & 0x80) != 0) {
        g_units[u].hold.n.turn_stamp = g_game.turn_counter;
        g_units[u].flags2 |= 8;
    }
    if ((best_flags & 0x8a) == 0 && coldist >= 0 &&
        (g_nation_population[g_colonies[colony].nation] >> 3) + ((coldist * 2 + 5) << 1) <=
            turnref - g_units[u].hold.n.turn_stamp) {
        g_units[u].dest_x = g_colonies[colony].x;
        g_units[u].dest_y = g_colonies[colony].y;
        step = move_to(u);
        if (step >= 0) {
            sx = g_dir_dx[step] + ux;
            sy = g_dir_dy[step] + uy;
            sq_nat = is_city(sx, sy);
            if (sq_nat >= 0 && sq_nat != nation)
                goto skip;
            owner = is_unit(sx, sy);
            if (owner >= 0 && owner != nation)
                goto skip;
            chosen = step;
            goto done;
        }
    }
skip:
    if ((best_flags & 0x1a) != 0)
        goto done;
    if ((best_flags & 1) != 0 && g_units[u].moves_used != 0) {
        chosen = 8;
        end_unit_turn(u);
        goto done;
    }
done:
    if (chosen == 8)
        end_unit_turn(u);
out:
    return chosen;
}
