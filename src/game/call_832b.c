/* call_832b -- shipped at 1050:83da.
 *
 * Ported by tools/port.py from matched/game/call_832b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_height_1050_832b(void *p);

/* same object: 4-byte `push cs; call ` */


void * call_832b(void *p)
{
    return rect_height_1050_832b(p);
}
