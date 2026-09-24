/* colony_window_redraw -- shipped at 1010:aed6.
 *
 * Ported by tools/port.py from matched/game/colony_window_redraw.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void colony_draw_buildings(ColChild *w, col_i16 a);
void colony_draw_carriers(ColChild *w, col_i16 a);
void colony_draw_colonists(ColChild *w, col_i16 a);
void colony_draw_panel6(ColChild *w, col_i16 a);
void colony_draw_status(ColChild *w);
void colony_draw_tiles(ColChild *w);
void colony_draw_warehouse(ColChild *w, col_i16 a);
void colony_draw_work_area(ColChild *w, col_i16 a);
void copy_all_to_screen_1010_e3e6(ColChild *w);

extern void set_current_colony(col_i16 c);
extern void colony_refresh_screen(void);
extern void colony_sort_colonists(void);
extern void colony_blit_cell_1010_bd3f(void *o, col_i16 x, col_i16 y, col_i16 w, col_i16 h);
extern void empty_1010_b03a(void *w, col_i16 a, col_i16 b);











void colony_window_redraw(ColChild *w)
{
    set_current_colony(w->colony);
    colony_refresh_screen();
    colony_sort_colonists();
    colony_draw_tiles(w);
    colony_blit_cell_1010_bd3f(w, 0, 0, 0x280, 0x1b8);
    colony_draw_status(w);
    colony_draw_work_area(w, 0);
    colony_draw_colonists(w, 0);
    colony_draw_warehouse(w, 0);
    empty_1010_b03a(w, 0, 0);
    colony_draw_carriers(w, 0);
    colony_draw_panel6(w, 0);
    colony_draw_buildings(w, 0);
    copy_all_to_screen_1010_e3e6(w);
}
