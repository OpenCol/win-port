/* ctl_get_text -- shipped at 1050:83b7.
 *
 * Ported by tools/port.py from matched/game/ctl_get_text.c.
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void get_window_text(col_i16 hwnd, col_i16 a, col_i16 b);

void ctl_get_text(DlgControl *c, col_i16 a, col_i16 b)
{
    get_window_text(c->hwnd, a, b);
}
