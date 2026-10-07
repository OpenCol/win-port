/* village_anger: candidate for 1038:06b6 (1378 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * name-source: sibling-build
 * size-source: 1378, not the census 1359 -- a dead second epilogue and the 10-byte jump table.
 *
 * WHICH EUROPEAN NATION HAS THE MOST PULL ON NATIVE SETTLEMENT `s`, and how much. Returns the
 * nation (or -1) and, when `strength` is not null, its score. The settlement renderer draws one "!"
 * per four points of it. Settlement turns, mission denunciations and `1028:5329` also ask.
 *
 * MILITARY PRESENCE. On each of the 20 land squares around the settlement whose top unit is European,
 * the units' attack values above 1 are added up (ships excluded). The sum is halved on a colony or village square (`is_city`)
 * and halved again beyond the inner ring, and it counts for that unit's nation.
 *
 * COLONIES within 6 squares (octile distance). Each scores
 *   `(min(pop / 2, tribe level) + 2 x pop above 6 + min(pop, 6) + bonus) x 2 - (distance + 1)`
 * divided by `distance + 4`.
 *   - The bonus counts the set bits of the colony's six bytes at +0x84, x mul / div, less 8, over 4.
 *     A human colony's mul/div by difficulty are 1/2, 3/4, 1/1, 3/2, 2/1, and it adds the difficulty
 *     itself.
 *   - The score is halved across a region boundary (plane-2 nibble), gains the nation's military
 *     presence, and is halved for France and again for Pocahontas.
 * The best colony's nation wins.
 *
 * THE MISSION then weighs in. A mission of another nation raises the winner's score by half, or
 * doubles it for an expert mission. The winner's own mission lowers it by a quarter, or halves it
 * for an expert one.
 *
 * `~x + 1` is the absolute value (rule 11), and the signed ring bytes are compared through `(int)`
 * (rule 51).
 *
 * RENAMED 2026-09-26 from settlement_influence: the Mac build's village_anger(short, short*), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller Stats; caller plan_move; callee has_father; callee land_region_at; callee on_map), the runner-up gets 3, and the arity agrees (2).
 */

#include "colonization.h"

#include "control.h"
#include "units.h"
#include "nations.h"
#include "fathers.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */
extern int land_region_at(int x, int y);

extern int terrain_is_ocean(int x, int y);
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);
extern int is_city(int x, int y);
extern int map_dist(int x0, int y0, int x1, int y1);
extern int has_father(int nation, int bit);
/* same object: 4-byte `push cs; call ` (rule 47) */

int village_anger(int s, int *strength)
{
    int best_nation;
    int score;
    int i;
    int dist;
    int best;
    int best_col;
    int colony;
    int dir;
    int sx;
    int sy;
    int nx;
    int ny;
    int u;
    int level;
    int bonus;
    int bits;
    int sum;
    int div;
    int mul;
    int force;
    int region;
    int cregion;
    int cnation;
    int unat;
    int pop;
    int p6;
    int rest;
    int mission;
    int near_force[4];
    register int k;
    register int bit;

    best_nation = -1;
    score = 0;
    best = 0;
    best_col = -1;
    sx = g_settlements[s].x;
    sy = g_settlements[s].y;
    region = land_region_at(sx, sy);
    for (i = 0; i < 4; i++)
        near_force[i] = 0;
    level = g_tribes[g_settlements[s].owner - 4].level;
    mission = g_settlements[s].mission;
    for (dir = 0; dir < 20; dir++) {
        nx = g_ring_dx[dir] + sx;
        ny = g_ring_dy[dir] + sy;
        if (!on_map(nx, ny))
            continue;
        if (terrain_is_ocean(nx, ny))
            continue;
        u = top_unit_in_square(nx, ny);
        if (u < 0)
            continue;
        unat = NATION(g_units[u]);
        if (unat >= 4)
            continue;
        for (force = 0; u >= 0; u = next_unit(u)) {
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
                continue;
            if (g_unit_types[g_units[u].type].attack > 1)
                force += g_unit_types[g_units[u].type].attack;
        }
        if (is_city(nx, ny) >= 0)
            force = force >> 1;
        if (((int)g_ring_dx[dir] > 0 ? (int)g_ring_dx[dir] : ~(int)g_ring_dx[dir] + 1) > 1
            || ((int)g_ring_dy[dir] > 0 ? (int)g_ring_dy[dir] : ~(int)g_ring_dy[dir] + 1) > 1)
            force = force >> 1;
        near_force[unat] += force;
    }
    for (colony = 0; g_game.colony_count > colony; colony++) {
        dist = map_dist(sx, sy, g_colonies[colony].x, g_colonies[colony].y);
        if (dist > 6)
            continue;
        cnation = g_colonies[colony].nation;
        cregion = land_region_at(g_colonies[colony].x, g_colonies[colony].y);
        bonus = 0;
        mul = 1;
        div = 1;
        if (cnation < NATION_EUROPEAN_COUNT && g_nation_text[cnation].control == CONTROL_HUMAN) {
            bonus += g_game.difficulty;
            switch (g_game.difficulty) {
            case 0:
                mul = 1;
                div = 2;
                break;
            case 1:
                mul = 3;
                div = 4;
                break;
            case 2:
                mul = 1;
                div = 1;
                break;
            case 3:
                mul = 3;
                div = 2;
                break;
            case 4:
                mul = 2;
                div = 1;
                break;
            }
        }
        sum = 0;
        for (i = 0; i < 6; i++) {
            bits = g_colonies[colony].flags[i];
            for (bit = 1, k = 0; k < 8; k++) {
                if (bits & bit)
                    sum += mul;
                bit = bit * 2;
            }
        }
        sum = sum / div;
        bonus += (sum - 8) >> 2;
        pop = g_colonies[colony].pop;
        p6 = min_int_1038_8680(pop, 6);
        rest = pop - p6;
        score = ((min_int_1038_8680(pop >> 1, level) + (rest * 2 + p6) + bonus) * 2 - (dist + 1)) / (dist + 4);
        if (region != cregion)
            score = score >> 1;
        score += near_force[cnation];
        if (cnation == NATION_FRANCE)
            score = score >> 1;
        if (has_father(cnation, FATHER_POCAHONTAS))
            score = score >> 1;
        if (score > best) {
            best = score;
            best_col = colony;
        }
    }
    if (best > 0) {
        best_nation = g_colonies[best_col].nation;
        if (mission >= 0) {
            if ((mission & 0xf) != best_nation) {
                if (mission & 0x10)
                    best = best * 2;
                else
                    best += best >> 1;
            } else {
                if (mission & 0x10)
                    best = best >> 1;
                else
                    best -= best >> 2;
            }
        }
    }
    if (strength)
        *strength = best;
    return best_nation;
}
