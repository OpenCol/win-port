/* MSWaitTicks -- shipped at 1078:184a.
 *
 * Ported by tools/port.py from matched/platform/MSWaitTicks.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i32 MSGetTicks(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

void MSWaitTicks(col_i16 n)
{
    col_i32 t = MSGetTicks();

    while (MSGetTicks() < n + t)
        ;
}
