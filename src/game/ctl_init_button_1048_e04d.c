/* ctl_init_button_1048_e04d -- shipped at 1048:e04d.
 *
 * Ported by tools/port.py from matched/game/ctl_init_button_1048_e04d.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: f20 -> w20
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void ctl_init_button_1048_e831(Ctl *c, void *win, col_i16 id, RECT *r,
                                   char *title);

/* same object: 4-byte `push cs; call ` */


void ctl_init_button_1048_e04d(Ctl *c, void *win, col_i16 id, RECT *r,
                                   col_i16 w20, char *title)
{
    c->w20 = w20;
    ctl_init_button_1048_e831(c, win, id, r, title);
}
