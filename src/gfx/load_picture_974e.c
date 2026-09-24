/* load_picture_974e -- shipped at 1068:363f.
 *
 * Ported by tools/port.py from matched/gfx/load_picture_974e.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void sub_1068_974e(void *p);

extern col_i16 load_picture_resource(void *p, col_i16 id, col_i16 first, col_i16 count, void *pal);

/* same object: 4-byte `push cs; call ` (rule 47) */


col_i16 load_picture_974e(void *p, col_i16 id, col_i16 first, col_i16 count, void *pal)
{
    sub_1068_974e(p);
    return load_picture_resource(p, id, first, count, pal);
}
