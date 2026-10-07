/* EuropeWin_Close: candidate for 1048:8d8e (95 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_Close
 *
 * MSShowMenuBar, then EuropeWin_Done on the object owning the current window; 0.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);
extern void MSShowMenuBar(void);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void EuropeWin_Done(EuropeWin *o);
int EuropeWin_Close(void)
{
    EuropeWin *o;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (EuropeWin *)w;

    MSShowMenuBar();
    if (o != 0)
        EuropeWin_Done(o);
    return 0;
}
