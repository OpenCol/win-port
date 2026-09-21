/* MSDoAllTasks -- shipped at 1078:11ae.
 *
 * Ported by tools/port.py from matched/platform/MSDoAllTasks.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_i;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 MSDoTask(void);

void MSDoAllTasks(void)
{
    while (MSDoTask())
        ;
}
