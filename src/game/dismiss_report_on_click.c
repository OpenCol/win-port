/* dismiss_report_on_click -- shipped at 1058:0c36.
 *
 * Ported by tools/port.py from matched/game/dismiss_report_on_click.c.
 *
 * The file's own ReportWin dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: flag -> no_accel
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void close_report_window(ReportWin *w);

extern char * current_window(void);
extern col_i16 desktop_fits_640(void *w);
/* same object: 4-byte `push cs; call ` (rule 47) */

void dismiss_report_on_click(col_i16 unused1, col_i16 unused2)
{
    ReportWin *c;
    char *w = current_window();

    c = (ReportWin *)(w != 0 ? w - 0x26 : w);
    if (c == 0)
        return;
    if (desktop_fits_640(c) != 0 || c->no_accel != 0)
        close_report_window(c);
}
