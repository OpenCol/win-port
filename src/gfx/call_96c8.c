/* call_96c8 -- shipped at 1068:99c8.
 *
 * Ported by tools/port.py from matched/gfx/call_96c8.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_height_1068_96c8(void *p);

/* same object: 4-byte `push cs; call ` */


void * call_96c8(void *p)
{
    return rect_height_1068_96c8(p);
}
