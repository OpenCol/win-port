/* copy_all_to_screen_1060_610c -- shipped at 1060:610c.
 *
 * Ported by tools/port.py from matched/game/copy_all_to_screen_1060_610c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_rect_to_screen_1060_631c(void *w, RECT *r);

/* same object: 4-byte `push cs; call ` */


void copy_all_to_screen_1060_610c(void *w)
{
    copy_rect_to_screen_1060_631c(w, 0);
}
