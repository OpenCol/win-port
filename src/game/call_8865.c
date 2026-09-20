/* call_8865 -- shipped at 1060:88e7.
 *
 * Ported by tools/port.py from matched/game/call_8865.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_width_1060_8865(void *p);

/* same object: 4-byte `push cs; call ` */


void * call_8865(void *p)
{
    return rect_width_1060_8865(p);
}
