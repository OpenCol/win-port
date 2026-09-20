/* call_d69b -- shipped at 1048:daca.
 *
 * Ported by tools/port.py from matched/game/call_d69b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_width_1048_d69b(void *p);

/* same object: 4-byte `push cs; call ` */


void * call_d69b(void *p)
{
    return rect_width_1048_d69b(p);
}
