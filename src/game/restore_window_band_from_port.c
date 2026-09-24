/* restore_window_band_from_port -- shipped at 1048:1bdf.
 *
 * Ported by tools/port.py from matched/game/restore_window_band_from_port.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void set_current_nation(col_i16 n);
extern void colony_blit_cell_1048_9d70(Win *w, col_i16 x, col_i16 y, col_i16 w2, col_i16 h);

void restore_window_band_from_port(Win *w)
{
    set_current_nation(w->nation);
    colony_blit_cell_1048_9d70(w, 0, -2, 0x280, 0xfa);
}
