/* GRPort_FillColor_all -- shipped at 1068:5154.
 *
 * Ported by tools/port.py from matched/gfx/GRPort_FillColor_all.c.
 *
 * The file's own GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void FillColor(GRPort *p, col_i16 *rect, col_i16 color);

/* same object: 4-byte `push cs; call ` (rule 47) */

void GRPort_FillColor_all(GRPort *p, col_i16 color)
{
    FillColor(p, ((col_i16 *)((unsigned char *)(p) + 0x8)), color);
}
