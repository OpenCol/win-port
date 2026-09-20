/* call_d6ba -- shipped at 1048:dae9.
 *
 * Ported by tools/port.py from matched/game/call_d6ba.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_height_1048_d6ba(void *p);

/* same object: 4-byte `push cs; call ` */


void * call_d6ba(void *p)
{
    return rect_height_1048_d6ba(p);
}
