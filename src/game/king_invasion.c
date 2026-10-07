/* PORT: this file's own declarations of MapWin_SlideIcon removed: the header declares it from the definition (W1 step 3). */
/* king_invasion: candidate for 1038:5473 (2637 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * THE ROYAL EXPEDITIONARY FORCE COMES ASHORE -- one wave, one colony, called once a turn by
 * `revolution_check` while the War of Independence runs. It picks the colony, picks the
 * sea square to anchor in, lands a Man-O-War there and then walks troops off it onto the beach until
 * the ship is empty or there is nowhere left to put them.
 *
 * THE FOUR COUNTERS at SEG20:0x77dc are regulars, cavalry, MEN-OF-WAR and artillery, in that order.
 * This function settles the last two, which the earlier reconstructions had the other way round: index
 * 2 is what is spent to put a unit of type 0x12 on an OCEAN square, and index 3 is one of the two
 * capped land kinds. With no ships at all the function does nothing but grant one -- provided the
 * the mother country's MAN_O_WAR tally is 0 -- and comes back next
 * turn.
 *
 * CHOOSING THE COLONY. Every colony of the player's with bit 6 of +0x1c set is scored, at most ten of
 * them: (125 - tolerance) times the colony's size, less 75 for each defender already standing on it,
 * and never less than (100 - tolerance) on its own. They are sorted by that score, and then each one's
 * defence strength is worked out and reduced by the units around it. The last entry -- the strongest
 * -- is tried first, and up to three passes walk backwards through the list until the force on hand
 * can actually take the target: a colony that needs more men than the King has left is skipped.
 *
 * CHOOSING THE ANCHORAGE. Of the eight squares around the colony, the sea squares that are real ocean
 * (plane-2 nibble 1) score one point for every adjacent unclaimed land square in the colony's own land
 * mass, and the best score wins. A square already holding another nation's Man-O-War is forced down to
 * a score of one rather than excluded.
 *
 * LANDING THE TROOPS. The ship's contents come off in two sweeps over the eight squares around it.
 * Squares already flying the mother country's flag take a unit straight away -- up to `cap` cavalry and
 * `cap` artillery per wave -- max(1, strength / 8), at most 2 once two regulars remain, 1 when
 * regulars outnumber the other two -- everything else regulars -- while enemy squares are only measured on the
 * first sweep, for the smallest defender count, and assaulted on the second. The wave ends when a
 * sweep places nothing and the second sweep has already run.
 *
 * TWO SHIPPED BUGS. The defence-strength pass reads `g_units[unit]` BEFORE `top_unit_in_square` is called for
 * that square, so its test of "is this the mother country's unit" uses whatever unit index the
 * previous loop left behind -- on the first square of the first colony that is the unit found at the
 * colony centre in the scoring loop above. And the anchorage loop tests `score < 0` to reject a
 * square, which cannot happen: `score` is only ever incremented from zero or set to one.
 *
 * WHAT THE BYTES SETTLED: `b = a = 100 - tolerance` is one chained assignment, not two statements --
 * the second store comes from the register, not from memory. `min_int` takes the counter FIRST and the
 * cap second, which is the reverse of how it reads. The two `max(1, x/8)` calls use rule 61's comma
 * form through `max_ptr_1038_8d92` and each gets its OWN pair of temporaries, four slots in all. The
 * distance checks are the corpus's `ABSDIFF` macro with rule 11's `~x + 1`. And `if (troops > 0)`
 * around the placement is a structured `if` (a long conditional jump), where the three tests before it
 * are `continue`s (short jumps over long ones) -- rule 12 both ways in the same loop body.
 *
 * RENAMED 2026-09-26 from royal_expedition_lands: the Mac build's king_invasion(short), bound by call-graph agreement (tools/mac_callgraph.py): 10 of its Mac call edges already bound land on this function (callee SlideIcon; callee colony_rebel; callee continent_at; callee force_mood; callee land_region_at), the runner-up gets 7, and the arity agrees (1).
 */

#include "colonization.h"

#include "royal.h"
#include "units.h"
#define ABSDIFF(a, b) ((a) - (b) > 0 ? (a) - (b) : ~((a) - (b)) + 1)

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

#include "classes.h"

extern void set_colony(int colony);
extern int colony_rebel(void);
extern void sort_16x16(int n, int *payload, int *key);
extern int terrain_is_ocean(int x, int y);
extern int is_city(int x, int y);
extern unsigned char continent_at(int x, int y);
extern int land_region_at(int x, int y);
extern char owner_of(int x, int y);
extern void set_center(int x, int y, int flag);
extern int popm(char *msg, int n);
extern void force_mood(int mood);
extern void DrawMapRegion(int x, int y, int a, int b, int flag);

/* 5-byte NOP form, so extern (rule 47) */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

extern int stack_check(int u, int kind);
extern int get_strength(void);
extern void purge_square(int x, int y);

extern void spot_unit(int u, int n);
extern void end_unit_turn(int u);
extern void relocate_stacked_units(int u, int x, int y);
extern int type_of_army(int kind, int nation);

void king_invasion(int nation)
{
    register int unit;
    register int nx;
    int colony;
    int k;
    int n;
    int dir;
    int d2;
    int region;
    int region2;
    int ny;
    int nx2;
    int ny2;
    int lx;
    int ly;
    int cx;
    int cy;
    int score;
    int best;
    int troops;
    int strength;
    int cnt3;
    int cnt1;
    int cap;
    int kind;
    int type;
    int placed;
    int found;
    int second;
    int least;
    int tally;
    int contested;
    int skip;
    int pass;
    int changed;
    int b;
    int a;
    int wipe;
    int total;
    int eighth;
    int one;
    int eighth2;
    int one2;
    int ids[10];
    int keys[10];
    int defence[10];

    wipe = 0;
    if (g_game.royal_forces[ROYAL_MAN_O_WAR] != 0) {
        total = g_game.royal_forces[ROYAL_REGULARS] + g_game.royal_forces[ROYAL_CAVALRY] + g_game.royal_forces[ROYAL_MAN_O_WAR] + g_game.royal_forces[ROYAL_ARTILLERY];
        if (total <= 4 || total - g_game.royal_forces[ROYAL_MAN_O_WAR] == 0)
            wipe = 1;
        if (g_game.royal_forces[ROYAL_MAN_O_WAR] == total)
            goto done;
    } else {
        if (g_unit_tally[g_game.mother_country_idx][UNIT_MAN_O_WAR] == 0)
            g_game.royal_forces[ROYAL_MAN_O_WAR]++;
        goto done;
    }
    n = 0;
    for (colony = 0; colony < g_game.colony_count; colony++) {
        set_colony(colony);
        if (g_cur_colony_rec->nation != (unsigned char)nation)
            continue;
        if (!(g_cur_colony_rec->status & 0x40))
            continue;
        if (n < 10) {
            b = a = 100 - colony_rebel();
            b = (b + 25) * (int)g_cur_colony_rec->pop;
            unit = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y);
            b -= stack_check(unit, 10) * 0x4b;
            b = max_int_1038_86a6(a, b);
            keys[n] = b;
            ids[n] = (unsigned char)colony;
            n++;
        }
    }
    if (n != 0)
        sort_16x16(n, ids, keys);
    for (k = 0; k < n; k++) {
        colony = ids[k];
        set_colony(colony);
        defence[k] = get_strength();
        for (dir = 0; dir < 8; dir++) {
            cx = g_cur_colony_rec->x + g_dir_dx[dir];
            cy = g_cur_colony_rec->y + g_dir_dy[dir];
            if (terrain_is_ocean(cx, cy))
                continue;
            if ((unsigned char)(g_units[unit].flags & UNIT_NATION_MASK)
                != (unsigned char)g_game.mother_country_idx)
                continue;
            unit = top_unit_in_square(cx, cy);
            while (unit >= 0 && defence[k] > 0) {
                if (g_unit_types[g_units[unit].type].attack != 0)
                    defence[k]--;
                unit = next_unit(unit);
            }
        }
    }
    changed = 1;
    for (pass = 0; changed != 0 && pass < 3; pass++) {
        skip = 0;
        while (skip < n && changed != 0) {
            changed = 0;
            colony = ids[n - 1 - skip];
            set_colony(colony);
            strength = defence[n - 1 - skip];
            troops = strength;
            troops = max_int_1038_86a6(1, troops);
            cap = *max_ptr_1038_8d92((one = 1, &one), (eighth = troops >> 3, &eighth));
            if (g_game.royal_forces[ROYAL_REGULARS] >= g_game.royal_forces[ROYAL_CAVALRY] + g_game.royal_forces[ROYAL_ARTILLERY])
                cap = 1;
            cnt1 = min_int_1038_8680(g_game.royal_forces[ROYAL_CAVALRY], cap);
            cnt3 = min_int_1038_8680(g_game.royal_forces[ROYAL_ARTILLERY], cap);
            if (pass != 0)
                troops = min_int_1038_8680(g_unit_types[UNIT_MAN_O_WAR].holds, troops);
            if (pass < 2) {
                if (g_game.royal_forces[ROYAL_REGULARS] + cnt1 + cnt3 < troops) {
                    skip++;
                    changed = 1;
                }
            }
        }
    }
    troops = min_int_1038_8680(g_unit_types[UNIT_MAN_O_WAR].holds, troops);
    if (skip >= n)
        goto done;
    region = land_region_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
    best = 0;
    for (dir = 0; dir < 8; dir++) {
        nx = g_cur_colony_rec->x + g_dir_dx[dir];
        ny = g_cur_colony_rec->y + g_dir_dy[dir];
        score = 0;
        if (!terrain_is_ocean(nx, ny))
            continue;
        if (continent_at(nx, ny) != 1)
            continue;
        for (d2 = 0; d2 < 8; d2++) {
            nx2 = g_dir_dx[d2] + nx;
            ny2 = g_dir_dy[d2] + ny;
            if (terrain_is_ocean(nx2, ny2))
                continue;
            if (land_region_at(nx2, ny2) != region)
                continue;
            if (is_city(nx2, ny2) >= 0)
                continue;
            score++;
        }
        if (score == 0)
            continue;
        unit = top_unit_in_square(nx, ny);
        if (unit >= 0 && (unsigned char)(g_units[unit].flags & UNIT_NATION_MASK)
                         != (unsigned char)g_game.mother_country_idx) {
            do {
                if (g_units[unit].type == UNIT_MAN_O_WAR)
                    score = 1;
                unit = next_unit(unit);
            } while (unit >= 0);
        }
        if (score < 0)
            continue;
        if (score > best) {
            best = score;
            lx = nx;
            ly = ny;
        }
    }
    if (best <= 0)
        goto done;
    purge_square(lx, ly);
    g_game.royal_forces[ROYAL_MAN_O_WAR]--;
    unit = create_unit(UNIT_MAN_O_WAR, g_game.mother_country_idx, lx, ly);
    if (unit < 0)
        goto done;
    spot_unit(unit, nation);
    set_center(lx, ly, 0);
    force_mood(3);
    set_name_slot_1038_8775(0, g_cur_colony_rec->name);
    popm("INVASION", 1);
    wipe = 0;
    cnt3 = 0;
    cnt1 = 0;
    cap = *max_ptr_1038_8d92((one2 = 1, &one2), (eighth2 = strength >> 3, &eighth2));
    if (g_game.royal_forces[ROYAL_REGULARS] >= 2)
        cap = min_int_1038_8680(2, cap);
    if (g_game.royal_forces[ROYAL_REGULARS] >= g_game.royal_forces[ROYAL_CAVALRY] + g_game.royal_forces[ROYAL_ARTILLERY])
        cap = 1;
    troops = max_int_1038_86a6(3, troops);
    found = 0;
    second = 0;
    least = 0x63;
    while (troops != 0) {
        placed = 0;
        for (dir = 0; dir < 8; dir++) {
            nx = g_dir_dx[dir] + lx;
            ny = g_dir_dy[dir] + ly;
            if (terrain_is_ocean(nx, ny))
                continue;
            if (is_city(nx, ny) >= 0)
                continue;
            if (ABSDIFF(nx, g_cur_colony_rec->x) > 1)
                continue;
            if (ABSDIFF(ny, g_cur_colony_rec->y) > 1)
                continue;
            region2 = land_region_at(nx, ny);
            if (region != region2)
                continue;
            if (troops > 0) {
                kind = 0;
                contested = 0;
                if ((unsigned char)owner_of(nx, ny) != (unsigned char)g_game.mother_country_idx) {
                    unit = top_unit_in_square(nx, ny);
                    tally = stack_check(unit, 2);
                    if (second == 0) {
                        if (tally < least) {
                            found = 1;
                            least = tally;
                        }
                        continue;
                    }
                    if (tally > least)
                        continue;
                    contested = 1;
                } else {
                    if (cnt1 < cap && g_game.royal_forces[ROYAL_CAVALRY] != 0) {
                        cnt1++;
                        kind = 1;
                    } else if (cnt3 < cap && g_game.royal_forces[ROYAL_ARTILLERY] != 0) {
                        cnt3++;
                        kind = 3;
                    }
                }
                if (g_game.royal_forces[kind] <= 0)
                    continue;
                type = type_of_army(kind, g_game.mother_country_idx);
                unit = create_unit(type, g_game.mother_country_idx, -2, -2);
                if (unit < 0) {
                    troops = 0;
                    break;
                }
                end_unit_turn(unit);
                spot_unit(unit, nation);
                MapWin_SlideIcon(&g_map_window, unit, 0xc0, -1, lx, ly, nx, ny);
                if (contested != 0) {
                    purge_square(nx, ny);
                    unit = top_unit_in_square(-2, -2);
                }
                relocate_stacked_units(unit, nx, ny);
                DrawMapRegion(g_cur_colony_rec->x - 2, g_cur_colony_rec->y - 2,
                                          5, 5, 1);
                g_game.royal_forces[kind]--;
                troops--;
                placed = 1;
            }
        }
        if (placed == 0) {
            if (found == 0 || second != 0)
                troops = 0;
            else
                second = 1;
        } else {
            second = 0;
            found = 0;
            least = 0x63;
        }
    }
done:
    if (wipe != 0) {
        for (kind = 0; kind < 4; kind++)
            g_game.royal_forces[kind] = 0;
    }
}
