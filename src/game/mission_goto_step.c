/* mission_goto_step -- shipped at 1028:ae3d.
 *
 * Ported by tools/port.py from matched/game/mission_goto_step.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern int far g_turn_nation;                        SEG20:0x7796
 *   extern unsigned char far g_view_nation;              SEG20:0x7798
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern int far g_mother_country_idx;                 SEG20:0x77d4
 *   extern int far g_moving_nation;                      DGROUP:0x10a4
 *
 * The file's own Unit, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "control.h"
#include "gameflag.h"
#include "terrain.h"
#include "nations.h"
#include "units.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */











extern col_i16 unit_next_step_dir(col_i16 u);
extern col_i16 unit_move_confirm(col_i16 dx, col_i16 dy);
extern void unit_move_to_square(col_i16 u, col_i16 x, col_i16 y);
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern void unit_stack_arrives(void);
extern void viceroy_unit_msg5(col_i16 u);
extern void unit_refresh_move_points(col_i16 u);

void mission_goto_step(register col_i16 u)
{
    col_i16 d;
    col_i16 n0;

    if (g_units[u].orders == 0xc)
        g_moving_nation = -1;
    else
        g_moving_nation = NATION(g_units[u]);
    d = unit_next_step_dir(u);
    if (d >= 0 && d < 8) {
        n0 = g_unit_count;
        if (NATION(g_units[u]) < 4 && g_nation52[NATION(g_units[u])].control == CONTROL_HUMAN) {
            if (unit_move_confirm(g_dir_dx[d], g_dir_dy[d]) != 0)
                g_units[u].orders = ORDERS_NONE;
        } else {
            unit_move_to_square(u, g_units[u].x + g_dir_dx[d],
                                       g_units[u].y + g_dir_dy[d]);
        }
        if (g_unit_count != n0)
            goto done;
        if (g_units[u].x == g_units[u].dest_x && g_units[u].y == g_units[u].dest_y) {
            if (terrain_at(g_units[u].x, g_units[u].y) == TERRAIN_SEA_LANE
                && g_units[u].orders != 0xc
                && ((g_turn_nation < NATION_EUROPEAN_COUNT && g_nation52[g_turn_nation].control == CONTROL_HUMAN)
                    || (col_i16)g_units[u].mission == 0x45)
                && ((g_game_flags & GAMEFLAG_INDEPENDENCE) == 0
                    || (g_turn_nation == g_mother_country_idx
                        && g_units[u].type == UNIT_MAN_O_WAR))) {
                g_selected_unit = u;
                unit_stack_arrives();
                if (NATION(g_units[u]) == g_view_nation)
                    viceroy_unit_msg5(g_selected_unit);
            }
            if (g_units[u].type == UNIT_PIONEERS) {
                g_units[u].f11 = 0;
                g_units[u].f12 = 0xff;
            }
            if (g_units[u].orders == ORDERS_MISSION)
                unit_refresh_move_points(u);
            if (g_units[u].orders != ORDERS_TRADE_ROUTE && g_units[u].orders != 0xc)
                g_units[u].orders = ORDERS_NONE;
        }
    } else {
        if (d != 8 || g_units[u].orders != ORDERS_TRADE_ROUTE)
            g_units[u].orders = ORDERS_NONE;
    }
done:
    g_moving_nation = -1;
}
