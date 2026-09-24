/* copy_all_to_screen_1030_e504 -- shipped at 1030:e504.
 *
 * Ported by tools/port.py from matched/game/copy_all_to_screen_1030_e504.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_rect_to_screen_1030_e699(void *w, RECT *r);

/* same object: 4-byte `push cs; call ` */


void copy_all_to_screen_1030_e504(void *w)
{
    copy_rect_to_screen_1030_e699(w, 0);
}
