/* ctl_init_button_1048_e831 -- shipped at 1048:e831.
 *
 * Ported by tools/port.py from matched/game/ctl_init_button_1048_e831.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void dlg_control_init_1048_e633(Ctl *c, void *win, col_i16 x, col_i16 y, RECT *r);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern col_i16 MSInitButton(RECT *r, Ctl *c, char *title, col_i16 style);
void ctl_init_button_1048_e831(Ctl *c, void *win, col_i16 y, RECT *r, char *title)
{
    dlg_control_init_1048_e633(c, win, 6, y, r);
    c->arg = 0;
    c->hbutton = MSInitButton(r, c, title, 1);
    c->state = 1;
}
