/* copy_all_to_screen_1038_e428 -- shipped at 1038:e428.
 *
 * Ported by tools/port.py from matched/game/copy_all_to_screen_1038_e428.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_rect_to_screen_1038_e38f(void *w, RECT *r);

/* same object: 4-byte `push cs; call ` */


void copy_all_to_screen_1038_e428(void *w)
{
    copy_rect_to_screen_1038_e38f(w, 0);
}
