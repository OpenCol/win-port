/* msg_box_1060_a772 -- shipped at 1060:a772.
 *
 * Ported by tools/port.py from matched/game/msg_box_1060_a772.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
MSWin * window_of(void *o);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void wait_mouse_button(MSWin *w, col_i16 a, col_i16 b, col_i16 c);
void msg_box_1060_a772(void *o, char own, col_i16 b, col_i16 c)
{
    if (own) {
        wait_mouse_button(window_of(o), 0, b, c);
        return;
    } else {
        wait_mouse_button(0, 0, b, c);
        return;
    }
}
