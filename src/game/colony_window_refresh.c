/* colony_window_refresh -- shipped at 1010:41be.
 *
 * Ported by tools/port.py from matched/game/colony_window_refresh.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_colony_window;                     SEG20:0x25be
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void colony_refresh_all(void);
extern void colony_draw_colonists(void *c, col_i16 a);
extern void colony_draw_buildings(void *c, col_i16 a);
extern void colony_draw_work_area(void *c, col_i16 a);
extern void empty_1010_b03a(void *c, col_i16 a, col_i16 b);
extern void colony_draw_panel6(void *c, col_i16 a);
extern void colony_draw_carriers(void *c, col_i16 a);
extern void colony_draw_warehouse(void *c, col_i16 a);
extern void colony_draw_status(void *c);

void colony_window_refresh(void)
{
    colony_refresh_all();
    colony_draw_colonists(&(*g_colony_window), 1);
    colony_draw_buildings(&(*g_colony_window), 1);
    colony_draw_work_area(&(*g_colony_window), 1);
    empty_1010_b03a(&(*g_colony_window), 1, 0);
    colony_draw_panel6(&(*g_colony_window), 1);
    colony_draw_carriers(&(*g_colony_window), 1);
    colony_draw_warehouse(&(*g_colony_window), 1);
    colony_draw_status(&(*g_colony_window));
}
