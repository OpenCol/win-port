/* anim_start_ex -- shipped at 1068:23a9.
 *
 * Ported by tools/port.py from matched/gfx/anim_start_ex.c.
 *
 * The file's own Anim dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void anim_start(Anim *a, void *port);

/* same object: 4-byte `push cs; call ` */


void anim_start_ex(Anim *a, void *port, col_i16 a4, col_i16 a5)
{
    a->f24e = a4;
    a->f250 = a5;
    anim_start(a, port);
}
