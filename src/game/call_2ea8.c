/* call_2ea8 -- shipped at 1010:2b62.
 *
 * Ported by tools/port.py from matched/game/call_2ea8.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_width_1010_2ea8(void *p);

/* same object: 4-byte `push cs; call ` */


void * call_2ea8(void *p)
{
    return rect_width_1010_2ea8(p);
}
