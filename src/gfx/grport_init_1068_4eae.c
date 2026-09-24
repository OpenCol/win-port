/* grport_init_1068_4eae -- shipped at 1068:4eae.
 *
 * Ported by tools/port.py from matched/gfx/grport_init_1068_4eae.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * The file's own GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 GRPort_Init(GRPort *p, RECT *r);

/* same object: 4-byte `push cs; call ` (rule 47) */


col_i16 grport_init_1068_4eae(GRPort *p, RECT *r)
{
    return GRPort_Init(p, r);
}
