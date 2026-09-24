/* refresh_unit_cursor -- shipped at 1008:3038.
 *
 * Ported by tools/port.py from matched/game/refresh_unit_cursor.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_view_mode;                          SEG20:0x7792
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern int g_cycle_flag;                             DGROUP:0x0076
 *   extern int far g_cursor_x;                           DGROUP:0x4b30
 *   extern int far g_cursor_y;                           DGROUP:0x4b32
 *   extern char far g_map_window[];                      SEG20:0x0658
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void set_cursor_pos(col_i16 x, col_i16 y);
extern col_i16 in_map_rect(col_i16 x, col_i16 y);
extern void refresh_arrived_unit_1040_0e2e(col_i16 a);
extern void draw_units_in_rect(col_i16 x, col_i16 y, col_i16 w, col_i16 h);
extern void map_draw_cursor(void *w, col_i16 x, col_i16 y);

void refresh_unit_cursor(void *unused1)
{
    if (g_view_mode == 0)
        set_cursor_pos(g_units[g_selected_unit].x, g_units[g_selected_unit].y);
    if (g_view_mode == 0
        && in_map_rect(g_units[g_selected_unit].x, g_units[g_selected_unit].y) != 0)
        refresh_arrived_unit_1040_0e2e(!g_cycle_flag);
    else
        draw_units_in_rect(g_cursor_x, g_cursor_y, 1, 1);
    if (g_view_mode != 0 && g_cycle_flag != 0)
        map_draw_cursor((&g_map_window), g_cursor_x, g_cursor_y);
}
