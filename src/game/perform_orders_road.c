/* perform_orders_road: candidate for 1028:a7e8 (823 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * One turn of a unit working a square -- `foreign_unit`'s order 9 (ORDERS_BUILD_ROAD). The work
 * counter at Unit+0x16 is bumped and compared against the terrain's own figure, byte 0 of the
 * 0x10-byte row at SEG20:0x78fc; speciality 0x14 HALVES it, which is the whole of the expert's
 * advantage. Below the figure nothing else happens; at or above it the work lands.
 *
 * Two bits of tile state gate the whole thing: bits 1 and 3 of map plane 1 mean the square is
 * already done, and then the only effect is to clear the unit's orders.
 *
 * WHEN THE WORK LANDS, five things follow: the counter and orders are cleared, the nearest
 * colony gets +10 at +0x98 if it belongs to this nation, the view scrolls and repaints IF the
 * player is watching this nation, bit 3 is set in the tile, and the countdown steps.
 *
 * THEN THE NATIVES NOTICE. If a village is within reach, the square is not already marked, no
 * European owns it, and the tribe's level is at least the distance, the nation pays for the
 * mark and the tribe's anger goes up by 3 plus the difficulty -- doubled within two squares and
 * TRIPLED within one, because both `if`s fire at distance 1.
 *
 * `u` IS COPIED INTO A REGISTER LOCAL. The shipped puts the parameter in SI and the local `x`
 * in DI, which is the opposite of what a `register` parameter and a `register` local give
 * (rule 53 hands SI to the local). `register int u = u_;` as the first local produces it.
 *
 * Six branches that look like early exits are DIRECT conditionals and so are nested `if`s, not
 * `goto`s (rule 12); two byte tests are the `!` form and so load and promote before testing
 * (rule 59); and the mask ternary is `>= 4 ? 0x8000 : 0x4000`, which the other order inverts.
 *
 * RENAMED 2026-09-26 from mission_work_step: the Mac build's perform_orders_road(short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller colony_plan; caller foreign_unit; callee end_unit_turn; callee feature_get; callee find_close_colony), the runner-up gets 6, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "terrain.h"
#include "nations.h"
#include "jobs.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameState g_game;   /* SEG20:0x7782 */

extern int land_region_at(int x, int y);
extern unsigned char *feature_loc(int x, int y);

extern void end_unit_turn(int u);
extern int find_close_colony(int x, int y, int nation, int land);
extern int possibly_center(int x1, int y1, int x2, int y2, int m);
extern void DrawMapRegion(int x, int y, int w, int h, int flag);
extern int find_close_village(int x, int y, int nation, int land);
extern unsigned char feature_get(int x, int y);
extern int is_colony(int x, int y);
extern int tribe_radius(int tribe);
extern void piss_off(int tribe, int nation, int alarm, int a);

/* same object: 4-byte `push cs; call ` */
extern void use_tools(int u);
extern int buy_this(int kind, int nation, int x, int y);

void perform_orders_road(int u_)
{
    register int u = u_;
    int y;
    int col;
    int nation;
    int vill;
    unsigned char *tile;
    int terr;
    int lvl;
    int alarm;
    int base;
    int need;
    int region;
    register int x;

    x = g_units[u].x;
    y = g_units[u].y;
    region = land_region_at(x, y);
    tile = feature_loc(x, y);
    terr = terrain_at(x, y);
    nation = NATION(g_units[u]);
    if (!(*tile & 0xa)) {
        end_unit_turn(u);
        g_units[u].counter++;
        need = g_terrain_rows[terr].work_turns;
        if ((int)g_units[u].spec == JOB_PIONEER)
            need = need >> 1;
        if (g_units[u].counter >= (unsigned char)need) {
            g_units[u].counter = 0;
            g_units[u].orders = ORDERS_NONE;
            if (g_game.colony_count != 0) {
                col = find_close_colony(x, y, -1, -1);
                if (NATION(g_units[u]) == g_cur_colony_rec->nation)
                    g_cur_colony_rec->bonus_hammers += 10;
            }
            if (g_game.turn_nation == g_game.view_nation
                || (g_game.reveal_all != 0
                    && (g_game.game_flags & (g_game.turn_nation >= NATION_EUROPEAN_COUNT ? 0x8000 : 0x4000)) != 0))
                possibly_center(x, y, x, y, 1);
            *tile |= 8;
            use_tools(u);
            if (g_game.turn_nation == g_game.view_nation
                || (g_game.reveal_all != 0
                    && (g_game.game_flags & (g_game.turn_nation >= NATION_EUROPEAN_COUNT ? 0x8000 : 0x4000)) != 0))
                DrawMapRegion(x - 1, y - 1, 3, 3, 1);
            vill = find_close_village(x, y, -1, region);
            if (vill >= 0
                && !((unsigned char)feature_get(x, y) & MAP1_CLAIM_SETTLED)
                && is_colony(x, y) < 0) {
                lvl = tribe_radius(g_cur_tribe);
                if (g_nearest_dist <= lvl
                    && buy_this(vill, nation, x, y) == 0) {
                    base = (g_game.turn_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_game.turn_nation].control == 0
                            ? g_game.difficulty : 0) + 3;
                    alarm = base;
                    if (g_nearest_dist <= 2)
                        alarm += base;
                    if (g_nearest_dist <= 1)
                        alarm += base;
                    piss_off(g_cur_tribe, nation, alarm, 1);
                }
            }
        }
    } else {
        g_units[u].orders = ORDERS_NONE;
    }
}
