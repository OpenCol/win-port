/* copy_all_to_screen_1060_ae72 -- shipped at 1060:ae72.
 *
 * Ported by tools/port.py from matched/game/copy_all_to_screen_1060_ae72.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_rect_to_screen_1060_afe6(void *w, RECT *r);

/* same object: 4-byte `push cs; call ` */


void copy_all_to_screen_1060_ae72(void *w)
{
    copy_rect_to_screen_1060_afe6(w, 0);
}
