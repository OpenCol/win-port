/* call_b01f -- shipped at 1020:ace9.
 *
 * Ported by tools/port.py from matched/game/call_b01f.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_width_1020_b01f(void *p);

/* same object: 4-byte `push cs; call ` */


void * call_b01f(void *p)
{
    return rect_width_1020_b01f(p);
}
