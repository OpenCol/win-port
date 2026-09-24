/* ReportWin_DoReport -- shipped at 1058:021c.
 *
 * Ported by tools/port.py from matched/game/ReportWin_DoReport.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 ReportWin_DoReport_parent(void *a, void *b, col_i32 c);

col_i16 ReportWin_DoReport(void *a, void *b)
{
    return ReportWin_DoReport_parent(a, b, 0L);
}
