/* ReportWin_Close: candidate for 1058:0b83 (91 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_Close
 *
 * ReportWin_Done on the object owning the current window, when there is one; 1.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
class SButton;
extern void *GetEventWindow(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

int ReportWin_Close(void)
{
    ReportWin *o;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ReportWin *)w;

    if (o != 0)
        ReportWin_Done(o);
    return 1;
}
