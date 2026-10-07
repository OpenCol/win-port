/* PORT: this file's own declarations of MapWin_SlideIcon removed: the header declares it from the definition (W1 step 3). */
/* allied_invasion: candidate for 1038:5ec0 (1480 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A FRIENDLY FORCE LANDS BESIDE ONE OF THE PLAYER'S COLONIES during the War of Independence:
 *   - `mercs == 0` is the INTERVENTION FORCE of the power at SEG20:0x77d6 (INTERVENE, sound
 *     SOUND_1812). It is drawn from the pool at SEG20:0x77e4 that `revolution`
 *     sized: regulars, dragoons, men-of-war, artillery;
 *   - `mercs != 0` is the MERCENARIES just paid for (MERCS, the power at 0x77d8). The counts come
 *     from the offer in data segment 26 (`buy_mercs`, `revolution_check`),
 *     and nothing is taken from the pool.
 *
 * WHERE. Only colonies with flag 0x40 of +0x1c count (the first ten), and one is chosen at random
 * weighted by population. The landing square is the adjacent ocean square of plane-2 region 1 that:
 *   - holds no royal (0x77d4) man-of-war;
 *   - is unowned or the player's;
 *   - has the most free, unowned land squares of the colony's own region around it.
 * With no such square, nothing arrives.
 *
 * WHAT. A MAN-O-WAR appears there and is scrolled to (`set_center`). For the intervention it is
 * taken from the pool. Then one ship-load of troops, at most six: up to two dragoons, up to two
 * artillery, and regulars for the rest (for mercenaries, exactly the offered counts). Each is created
 * off the map, made a veteran (JOB_SOLDIER), slid from the ship into the colony
 * (`MapWin_SlideIcon`) and linked there, and the intervention counts go down one by one. A failed
 * creation stops the landing. The mercenaries' man-of-war is only transport: it is freed afterwards.
 *
 * It compiled RELOCATABLE on the first build; `!(f1c & 0x40)` is the promoted test that
 * `intervention` also needed.
 *
 * RENAMED 2026-09-26 from intervention_force_arrives: the Mac build's allied_invasion(short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (callee SlideIcon; callee continent_at; callee land_region_at; callee next_unit; callee popm), the runner-up gets 7, and the arity agrees (1).
 */

#include "colonization.h"

#include "royal.h"
#include "units.h"
#include "jobs.h"
#include "sounds.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameState g_game;   /* SEG20:0x7782 */

#include "classes.h"

extern void set_colony(int colony);

extern int land_region_at(int x, int y);
extern unsigned char continent_at(int x, int y);
extern int terrain_is_ocean(int x, int y);
extern int is_unit(int x, int y);
extern int is_city(int x, int y);
extern void set_center(int x, int y, int flag);
extern void set_mood(int mood);

extern void popup_say(int slot, int word);
extern void sound_play(int n);
extern int popm(char *msg, int n);
extern void DrawMapRegion(int x, int y, int w, int h, int flag);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

extern void spot_unit(int u, int n);

extern int type_of_army(int kind, int nation);
extern void relocate_stacked_units(int u, int x, int y);
extern void delete_unit(int u);

void allied_invasion(int mercs)
{
    int nation;
    int owner;
    int colony;
    int n;
    int dir;
    int dir2;
    int region;
    int sx;
    int sy;
    int nx;
    int ny;
    int land_x;
    int land_y;
    int score;
    int best;
    int ship;
    int ok;
    int kind;
    int type;
    int count;
    int i;
    int total;
    int roll;
    int cap[4];
    unsigned char idx[10];
    int pop[10];
    register int u;

    nation = g_game.player_nation;
    n = 0;
    total = 0;
    for (colony = 0; g_game.colony_count > colony; colony++) {
        set_colony(colony);
        if (g_cur_colony_rec->nation != (unsigned char)nation)
            continue;
        if (!(g_cur_colony_rec->status & 0x40))
            continue;
        if (n < 10) {
            pop[n] = g_cur_colony_rec->pop;
            total += g_cur_colony_rec->pop;
            idx[n] = colony;
            n++;
        }
    }
    if (n == 0)
        goto out;
    roll = rand_range(1, total);
    colony = -1;
    for (i = 0; colony < 0 && i < n; i++) {
        roll -= pop[i];
        if (roll <= 0)
            colony = idx[i];
    }
    if (colony < 0)
        goto out;
    set_colony(colony);
    region = land_region_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
    best = 0;
    for (dir = 0; dir < 8; dir++) {
        sx = g_cur_colony_rec->x + g_dir_dx[dir];
        sy = g_cur_colony_rec->y + g_dir_dy[dir];
        score = 1;
        if (!terrain_is_ocean(sx, sy))
            continue;
        if (continent_at(sx, sy) != 1)
            continue;
        u = top_unit_in_square(sx, sy);
        if (u >= 0 && NATION(g_units[u]) == (unsigned char)g_game.mother_country_idx) {
            do {
                if (g_units[u].type == UNIT_MAN_O_WAR)
                    score -= 999;
                u = next_unit(u);
            } while (u >= 0);
        }
        if (score < 0)
            continue;
        owner = is_unit(sx, sy);
        if (owner >= 0 && owner != nation)
            continue;
        for (dir2 = 0; dir2 < 8; dir2++) {
            nx = g_dir_dx[dir2] + sx;
            ny = g_dir_dy[dir2] + sy;
            if (terrain_is_ocean(nx, ny))
                continue;
            if (land_region_at(nx, ny) != region)
                continue;
            if (is_city(nx, ny) >= 0)
                continue;
            score++;
        }
        if (score > best) {
            best = score;
            land_x = sx;
            land_y = sy;
        }
    }
    if (best <= 0)
        goto out;
    if (mercs == 0)
        g_game.force[ROYAL_MAN_O_WAR]--;
    ship = u = create_unit(UNIT_MAN_O_WAR, nation, land_x, land_y);
    if (u < 0)
        goto out;
    spot_unit(u, nation);
    set_center(land_x, land_y, 0);
    set_name_slot_1038_8775(0, g_cur_colony_rec->name);
    if (mercs == 0) {
        set_mood(3);
        popup_say(1, get_adjective(g_game.intervention_nation));
        sound_play(SOUND_1812);
        popm("INTERVENE", 1);
    } else {
        popup_say(1, get_adjective(g_game.merc_nation));
        popm("MERCS", 1);
    }
    ok = 1;
    cap[0] = 6;
    cap[1] = 0;
    cap[3] = 0;
    if (g_game.force[ROYAL_CAVALRY] != 0)
        cap[1] = min_int_1038_8680(g_game.force[ROYAL_CAVALRY], 2);
    if (g_game.force[ROYAL_ARTILLERY] != 0)
        cap[3] = min_int_1038_8680(g_game.force[ROYAL_ARTILLERY], 2);
    cap[0] -= cap[3] + cap[1];
    for (kind = 0; ok != 0 && kind <= 3; kind++) {
        if (kind == 2)
            continue;
        count = cap[kind];
        count = min_int_1038_8680(g_game.force[kind], count);
        type = type_of_army(kind, nation);
        if (mercs != 0)
            count = g_merc[kind];
        for (i = 0; i < count; i++) {
            u = create_unit(type, nation, -2, -2);
            if (u < 0) {
                ok = 0;
                break;
            }
            g_units[u].spec = JOB_SOLDIER;
            spot_unit(u, nation);
            MapWin_SlideIcon(&g_map_window, u, 0xc0, -1, land_x, land_y,
                             g_cur_colony_rec->x, g_cur_colony_rec->y);
            relocate_stacked_units(u, g_cur_colony_rec->x, g_cur_colony_rec->y);
            DrawMapRegion(g_cur_colony_rec->x - 2, g_cur_colony_rec->y - 2, 5, 5, 1);
            if (mercs == 0)
                g_game.force[kind]--;
        }
    }
    if (mercs != 0 && ship >= 0)
        delete_unit(ship);
out:
    ;
}
