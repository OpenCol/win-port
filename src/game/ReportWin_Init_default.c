/* ReportWin_Init_default -- shipped at 1058:0000.
 *
 * Ported by tools/port.py from matched/game/ReportWin_Init_default.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void ReportWin_Init(void *o, col_i16 flag, void *rec);

void ReportWin_Init_default(void *o)
{
    ReportWin_Init(o, 0, 0);
}
