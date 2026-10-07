/* orders_build: candidate for 1008:0953 (1068 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * THE "BUILD COLONY" ORDER for the selected unit (SEG20:0x7794): every reason not to, then the
 * order itself. It has no direct caller, so it is reached through a command table.
 *
 * On a square that already holds one of the player's colonies it does not build. It hands the
 * unit to that colony (`colony_add_unit`) and redraws the map.
 *
 * Otherwise the refusals and warnings run in this order:
 *   - NOCOLONIESEITHER once the revolution has begun (bit 0 of SEG20:0x7784);
 *   - TOOMANYCOLONIES at 48 colonies in the world or 38 for the unit's nation;
 *   - SEACOLONY on water;
 *   - TOONEAR when the nearest colony is one square away, named from the current colony record;
 *   - TOOMOUNTAIN on mountains;
 *   - ARCTICCIRCLE on the top row and ANTARTICA on the bottom (y 0x47);
 *   - TOONEARBUILD when any unit on the 3x3 block is already under ORDERS_BUILD_COLONY.
 * Each of those ends it. The next three are questions, and only answer 2 goes on:
 *   - NOPORT unless the adjacent ocean square found has plane-2 nibble 1;
 *   - below Conquistador only, TUTNOSPACES when fewer than four squares of the 3x3 block are
 *     usable (not water, desert or arctic, and with no native village on it; a special resource
 *     counts once more);
 *   - below Conquistador only, TUTNOLUMBER when none of the block is forest (terrain 8..23).
 * Then the unit's orders become ORDERS_BUILD_COLONY and `perform_orders_build` founds it.
 *
 * The neighbour scan reuses the selected-unit register for the unit chain on each square
 * and reloads it from SEG20:0x7794 afterwards. That is the shipped's own economy, and the
 * reload is in the bytes. It compiled RELOCATABLE on the first build.
 *
 * RENAMED 2026-09-26 from build_colony: the Mac build's orders_build(void), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller KeyboardEvent; callee perform_orders_build; callee terrain_at; callee top_unit_in_square), the runner-up gets 2, and the arity agrees (0).
 */

#include "colonization.h"

#include "gameflag.h"
#include "terrain.h"
#include "units.h"
#include "difficul.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameState g_game;   /* SEG20:0x7782 */

extern int popm(char *msg, int n);
extern int terrain_is_ocean(int x, int y);
extern int find_close_colony(int x, int y, int nation, int land);

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);
extern int is_village(int x, int y);
extern int resource_at(int x, int y);
extern int water_access(int x, int y);
extern unsigned char continent_at(int x, int y);
extern void perform_orders_build(int u);
extern int colony_add_unit(int colony, int u);
extern void DrawMap(int full);
/* same object: 4-byte `push cs; call ` (rule 47) */

void orders_build(void)
{
    int col;
    int unit;
    int nearest;
    int dir;
    int port;
    int nx;
    int ny;
    int land;
    int forest;
    register int u;
    register int terrain;

    u = g_game.selected_unit;
    col = find_colony(g_units[u].x, g_units[u].y);
    unit = u;
    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
        popm("NOCOLONIESEITHER", 1);
        goto out;
    }
    if (g_game.colony_count < 0x30 && g_nation_colony_count[NATION(g_units[u])] < 0x26) {
        if (col < 0) {
            if (terrain_is_ocean(g_units[u].x, g_units[u].y)) {
                popm("SEACOLONY", 0);
                goto out;
            }
            nearest = find_close_colony(g_units[u].x, g_units[u].y, -1, -1);
            if (nearest >= 0 && g_nearest_dist == 1) {
                set_name_slot_1008_cdca(0, g_cur_colony_rec->name);
                popm("TOONEAR", 5);
                goto out;
            }
            if (terrain_at(g_units[u].x, g_units[u].y) == TERRAIN_MOUNTAINS) {
                popm("TOOMOUNTAIN", 3);
                goto out;
            }
            if (g_units[u].y == 0) {
                popm("ARCTICCIRCLE", 3);
                goto out;
            }
            if (g_units[u].y == 0x47) {
                popm("ANTARTICA", 3);
                goto out;
            }
            land = 0;
            forest = 0;
            for (dir = 0; dir < 9; dir++) {
                nx = g_units[u].x + g_dir_dx[dir];
                ny = g_units[u].y + g_dir_dy[dir];
                for (u = top_unit_in_square(nx, ny); u >= 0; u = next_unit(u)) {
                    if (g_units[u].orders == ORDERS_BUILD_COLONY) {
                        popm("TOONEARBUILD", 3);
                        goto out;
                    }
                }
                u = g_game.selected_unit;
                terrain = terrain_at(nx, ny);
                if (terrain != TERRAIN_OCEAN && terrain != TERRAIN_SEA_LANE && terrain != TERRAIN_DESERT
                    && terrain != TERRAIN_ARCTIC && is_village(nx, ny) < 0)
                    land++;
                if (resource_at(nx, ny) != -1)
                    land++;
                if ((terrain >= TERRAIN_BOREAL_FOREST && terrain < TERRAIN_BOREAL_FOREST_2)
                    || (terrain >= TERRAIN_BOREAL_FOREST_2 && terrain < TERRAIN_ARCTIC))
                    forest = 1;
            }
            port = water_access(g_units[u].x, g_units[u].y);
            if (port != 0)
                port = continent_at(g_ocean_x, g_ocean_y) == 1 ? 1 : 0;
            if (port == 0) {
                if (popm("NOPORT", 3) != 2)
                    goto out;
            }
            if (g_game.difficulty < DIFFICULTY_CONQUISTADOR) {
                if (land < 4) {
                    if (popm("TUTNOSPACES", 3) != 2)
                        goto out;
                }
                if (forest == 0) {
                    if (popm("TUTNOLUMBER", 3) != 2)
                        goto out;
                }
            }
            g_units[u].orders = ORDERS_BUILD_COLONY;
            perform_orders_build(u);
        } else if (g_colonies[col].nation == ((unsigned char)g_game.turn_nation)) {
            if (colony_add_unit(col, unit) == 0) {
                unit = -1;
                DrawMap(1);
            }
        }
    } else {
        show_text_dialog_1008_cdf3("TOOMANYCOLONIES");
    }
out:
    ;
}
