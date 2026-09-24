/* map_goto -- shipped at 1038:afe2.
 *
 * Ported by tools/port.py from matched/game/map_goto.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_view_mode;                          SEG20:0x7792
 *   extern int far g_cycle_flag;                         DGROUP:0x0076
 *   extern char far g_game_record2[];                    SEG20:0x40d6
 *   extern int far g_map_center_x;                       DGROUP:0x1c92
 *   extern int far g_map_center_y;                       DGROUP:0x1c94
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void set_cursor_pos(col_i16 x, col_i16 y);

extern col_i16 on_map(col_i16 x, col_i16 y);
extern void refresh_unit_cursor(void *rec);
extern void redraw_map_1040_11b7(col_i16 a);



void map_goto(col_i16 x, col_i16 y, col_i16 flag)
{
    if (on_map(x, y) != 0) {
        if (g_view_mode == 1) {
            if (flag != 0 && g_cycle_flag != 0)
                refresh_unit_cursor(g_game_record2);
            set_cursor_pos(x, y);
            if (flag != 0) {
                g_cycle_flag = 0;
                refresh_unit_cursor(g_game_record2);
            }
        }
        g_map_center_x = x;
        g_map_center_y = y;
        redraw_map_1040_11b7(1);
    }
}
