/* control_show_1010_e574 -- shipped at 1010:e574.
 *
 * Ported by tools/port.py from matched/game/control_show_1010_e574.c.
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void show_hwnd(col_i16 h);
extern void invalidate_hwnd(col_i16 h);

void control_show_1010_e574(DlgControl *c)
{
    if (c->hwnd != 0) {
        show_hwnd(c->hwnd);
        invalidate_hwnd(c->hwnd);
    }
}
