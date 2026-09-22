/* ctl_sel_text -- shipped at 1050:8d3a.
 *
 * Ported by tools/port.py from matched/game/ctl_sel_text.c.
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void listbox_sel_text(col_i16 hwnd, col_i16 a, col_i16 b);

void ctl_sel_text(DlgControl *c, col_i16 a, col_i16 b)
{
    listbox_sel_text(c->hwnd, a, b);
}
