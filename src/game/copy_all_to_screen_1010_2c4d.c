/* copy_all_to_screen_1010_2c4d -- shipped at 1010:2c4d.
 *
 * Ported by tools/port.py from matched/game/copy_all_to_screen_1010_2c4d.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_rect_to_screen_1010_2f5a(void *w, RECT *r);

/* same object: 4-byte `push cs; call ` */


void copy_all_to_screen_1010_2c4d(void *w)
{
    copy_rect_to_screen_1010_2f5a(w, 0);
}
