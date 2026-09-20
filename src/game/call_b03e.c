/* call_b03e -- shipped at 1020:ad08.
 *
 * Ported by tools/port.py from matched/game/call_b03e.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_height_1020_b03e(void *p);

/* same object: 4-byte `push cs; call ` */


void * call_b03e(void *p)
{
    return rect_height_1020_b03e(p);
}
