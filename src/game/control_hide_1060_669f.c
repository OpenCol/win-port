/* control_hide_1060_669f -- shipped at 1060:669f.
 *
 * Ported by tools/port.py from matched/game/control_hide_1060_669f.c.
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void hide_hwnd(col_i16 h);

void control_hide_1060_669f(DlgControl *c)
{
    if (c->hwnd != 0)
        hide_hwnd(c->hwnd);
}
