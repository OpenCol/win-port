/* control_hide_1010_e54d -- shipped at 1010:e54d.
 *
 * Ported by tools/port.py from matched/game/control_hide_1010_e54d.c.
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void hide_hwnd(col_i16 h);

void control_hide_1010_e54d(DlgControl *c)
{
    if (c->hwnd != 0)
        hide_hwnd(c->hwnd);
}
