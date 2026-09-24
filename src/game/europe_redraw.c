/* europe_redraw -- shipped at 1048:8cc9.
 *
 * Ported by tools/port.py from matched/game/europe_redraw.c.
 *
 * The file's own EuropeWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_all_to_screen_1048_dd24(EuropeWin *w);
void reset_chain_counts(void);
void set_current_nation(col_i16 n);

extern void colony_blit_cell_1048_9d70(EuropeWin *w, col_i16 x, col_i16 y, col_i16 cx, col_i16 cy);
extern void europe_draw_market(EuropeWin *w, col_i16 a);
extern void europe_draw_status(EuropeWin *w);
extern void europe_draw_dock_1048_91ac(EuropeWin *w, col_i16 a);
extern void sub_1048_990b(EuropeWin *w, col_i16 a);
extern void europe_draw_dock_1048_9df1(EuropeWin *w, col_i16 a);

/* same object: 4-byte `push cs; call ` */




void europe_redraw(EuropeWin *w)
{
    colony_blit_cell_1048_9d70(w, 0, 0, 0x280, 0x1e0);
    set_current_nation(w->nation);
    reset_chain_counts();
    europe_draw_market(w, 0);
    europe_draw_status(w);
    europe_draw_dock_1048_91ac(w, 0);
    sub_1048_990b(w, 0);
    europe_draw_dock_1048_9df1(w, 0);
    copy_all_to_screen_1048_dd24(w);
}
