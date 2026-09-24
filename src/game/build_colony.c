/* build_colony -- shipped at 1008:0953.
 *
 * Ported by tools/port.py from matched/game/build_colony.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern unsigned char far g_turn_nation;              SEG20:0x7796
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern unsigned char far g_nation_colony_count[4];   DGROUP:0x4b48
 *   extern int far g_nearest_dist;                       DGROUP:0x4bc6
 *   extern int far g_ocean_x;                            DGROUP:0x4bc8
 *   extern int far g_ocean_y;                            DGROUP:0x4bca
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *
 * The file's own Unit, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void set_name_slot_1008_cdca(col_i16 slot, char *s);
void show_text_dialog_1008_cdf3(char *id);

#include "gameflag.h"
#include "terrain.h"
#include "units.h"
#include "difficul.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))
extern col_i16 colony_at(col_i16 x, col_i16 y);
extern col_i16 text_message_show(char *msg, col_i16 n);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 find_nearest_colony(col_i16 x, col_i16 y, col_i16 nation, col_i16 land);
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern col_i16 unit_at(col_i16 x, col_i16 y);
extern col_i16 unit_chain_next(col_i16 u);
extern col_i16 square_native_nation(col_i16 x, col_i16 y);
extern col_i16 tile_decoration(col_i16 x, col_i16 y);
extern col_i16 find_adjacent_ocean(col_i16 x, col_i16 y);
extern unsigned char square_region(col_i16 x, col_i16 y);
extern void mission_found_colony(col_i16 u);
extern col_i16 colony_employ_unit(col_i16 col, col_i16 u);
extern void redraw_map_1040_11b7(col_i16 full);
/* same object: 4-byte `push cs; call ` (rule 47) */



void build_colony(void)
{
    col_i16 col;
    col_i16 unit;
    col_i16 nearest;
    col_i16 dir;
    col_i16 port;
    col_i16 nx;
    col_i16 ny;
    col_i16 land;
    col_i16 forest;
    register col_i16 u;
    register col_i16 t;

    u = g_selected_unit;
    col = colony_at(g_units[u].x, g_units[u].y);
    unit = u;
    if (g_game_flags & GAMEFLAG_INDEPENDENCE) {
        text_message_show("NOCOLONIESEITHER", 1);
        goto out;
    }
    if (g_colony_count < 0x30 && g_nation_colony_count[NATION(g_units[u])] < 0x26) {
        if (col < 0) {
            if (ocean(g_units[u].x, g_units[u].y)) {
                text_message_show("SEACOLONY", 0);
                goto out;
            }
            nearest = find_nearest_colony(g_units[u].x, g_units[u].y, -1, -1);
            if (nearest >= 0 && g_nearest_dist == 1) {
                set_name_slot_1008_cdca(0, ((char *)((unsigned char *)(g_cur_colony_rec) + 0x2)));
                text_message_show("TOONEAR", 5);
                goto out;
            }
            if (terrain_at(g_units[u].x, g_units[u].y) == TERRAIN_MOUNTAINS) {
                text_message_show("TOOMOUNTAIN", 3);
                goto out;
            }
            if (g_units[u].y == 0) {
                text_message_show("ARCTICCIRCLE", 3);
                goto out;
            }
            if (g_units[u].y == 0x47) {
                text_message_show("ANTARTICA", 3);
                goto out;
            }
            land = 0;
            forest = 0;
            for (dir = 0; dir < 9; dir++) {
                nx = g_units[u].x + g_dir_dx[dir];
                ny = g_units[u].y + g_dir_dy[dir];
                for (u = unit_at(nx, ny); u >= 0; u = unit_chain_next(u)) {
                    if (g_units[u].orders == ORDERS_BUILD_COLONY) {
                        text_message_show("TOONEARBUILD", 3);
                        goto out;
                    }
                }
                u = g_selected_unit;
                t = terrain_at(nx, ny);
                if (t != TERRAIN_OCEAN && t != TERRAIN_SEA_LANE && t != TERRAIN_DESERT
                    && t != TERRAIN_ARCTIC && square_native_nation(nx, ny) < 0)
                    land++;
                if (tile_decoration(nx, ny) != -1)
                    land++;
                if ((t >= TERRAIN_BOREAL_FOREST && t < TERRAIN_BOREAL_FOREST_2)
                    || (t >= TERRAIN_BOREAL_FOREST_2 && t < TERRAIN_ARCTIC))
                    forest = 1;
            }
            port = find_adjacent_ocean(g_units[u].x, g_units[u].y);
            if (port != 0)
                port = square_region(g_ocean_x, g_ocean_y) == 1 ? 1 : 0;
            if (port == 0) {
                if (text_message_show("NOPORT", 3) != 2)
                    goto out;
            }
            if (g_difficulty < DIFFICULTY_CONQUISTADOR) {
                if (land < 4) {
                    if (text_message_show("TUTNOSPACES", 3) != 2)
                        goto out;
                }
                if (forest == 0) {
                    if (text_message_show("TUTNOLUMBER", 3) != 2)
                        goto out;
                }
            }
            g_units[u].orders = ORDERS_BUILD_COLONY;
            mission_found_colony(u);
        } else if (g_colonies[col].nation == g_turn_nation) {
            if (colony_employ_unit(col, unit) == 0) {
                unit = -1;
                redraw_map_1040_11b7(1);
            }
        }
    } else {
        show_text_dialog_1008_cdf3("TOOMANYCOLONIES");
    }
out:
    ;
}
