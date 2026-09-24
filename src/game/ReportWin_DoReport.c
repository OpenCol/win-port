/* ReportWin_DoReport -- shipped at 1058:021c.
 *
 * Ported by tools/port.py from matched/game/ReportWin_DoReport.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 ReportWin_DoReport_parent(void *o, col_i16 kind, char flag,
                                         void *rec);

col_i16 ReportWin_DoReport(void *o, col_i16 kind, char flag)
{
    return ReportWin_DoReport_parent(o, kind, flag, 0);
}
