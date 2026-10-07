/* ReportWin_Init_default: candidate for 1058:0000 (33 bytes).  Verdict: RELOCATABLE.
 *
 * ReportWin_Init with the defaults: the plain form, the default record. The call is a call
 * TLINK converted to its NOP-padded `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void ReportWin_Init(ReportWin *o, char flag, void *rec);

void ReportWin_Init_default(ReportWin *o)
{
    ReportWin_Init(o, 0, 0);
}
