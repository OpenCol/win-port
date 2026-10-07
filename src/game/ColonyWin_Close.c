/* ColonyWin_Close: candidate for 1010:afa9 (91 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_Close
 *
 * ColonyWin_Done on the object owning the current window, when there is one; 0.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);
extern void ColonyWin_Done(void *o);

int ColonyWin_Close(void)
{
    ColonyWin *o;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ColonyWin *)w;

    if (o != 0)
        ColonyWin_Done(o);
    return 0;
}
