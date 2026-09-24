/* ReportWin_Close -- shipped at 1058:0b83.
 *
 * Ported by tools/port.py from matched/game/ReportWin_Close.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void close_report_window(void *o);

extern char * current_window(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 ReportWin_Close(void)
{
    char *o;
    char *w = current_window();

    o = w != 0 ? w - 0x26 : w;

    if (o != 0)
        close_report_window(o);
    return 1;
}
