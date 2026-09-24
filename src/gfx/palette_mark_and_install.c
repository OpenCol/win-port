/* palette_mark_and_install -- shipped at 1068:0598.
 *
 * Ported by tools/port.py from matched/gfx/palette_mark_and_install.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void palette_install(void *p, col_i16 start, col_i16 count);

extern void palette_mark_range_reserved(void *p, col_i16 start, col_i16 count);
/* same object: 4-byte `push cs; call ` (rule 47) */

void palette_mark_and_install(void *p, col_i16 start, col_i16 count)
{
    palette_mark_range_reserved(p, start, count);
    palette_install(p, start, count);
}
