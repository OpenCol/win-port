/* wipe_rect -- shipped at 1068:c1e9.
 *
 * Ported by tools/port.py from matched/gfx/wipe_rect.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void wipe_rows(void *port, void *w, RECT *r,
                                    col_i16 x, col_i16 y, col_i16 mode, col_i16 step, col_i16 delay);

void wipe_rect(void *port, void *w, RECT *r,
                             col_i16 x, col_i16 y, col_i16 mode, col_i16 step)
{
    wipe_rows(port, w, r, x, y, mode, step, 0);
}
