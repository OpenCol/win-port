/* get_mouse_pos_for_1050_8f2b -- shipped at 1050:8f2b.
 *
 * Ported by tools/port.py from matched/game/get_mouse_pos_for_1050_8f2b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * get_p_6_1050_94fb(void *a);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void MSGetMousePos(void *w, void *b, void *c);

void get_mouse_pos_for_1050_8f2b(void *a, void *b, void *c)
{
    MSGetMousePos(get_p_6_1050_94fb(a), b, c);
}
