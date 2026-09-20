/* call_830c -- shipped at 1050:841c.
 *
 * Ported by tools/port.py from matched/game/call_830c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_width_1050_830c(void *p);

/* same object: 4-byte `push cs; call ` */


void * call_830c(void *p)
{
    return rect_width_1050_830c(p);
}
