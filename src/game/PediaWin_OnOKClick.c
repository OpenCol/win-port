/* PediaWin_OnOKClick: candidate for 1060:2b6b (88 bytes).  Verdict: RELOCATABLE.
 * symbol: PediaWin_OnOKClick
 * name-source: sibling-build
 *
 * PediaWin_Done on the GameWin that owns the current window (its DialogWin
 * is 0x26 in), when there is one.  The callee is -called and defined
 * in-file as a stub.
 *
 * RENAMED 2026-09-26 from current_window_call: the Mac build's PediaWin::OnOKClick(short): PediaWin_Init installs it as the action of the window's image button, and it closes the current Colonopedia with PediaWin_Done -- the counterpart of ColonyWin_OnOKClick and EuropeWin_OnOKClick. The body reads no argument, so the Windows prototype has none.
 */

#include "colonization.h"

#include "w16types.h"
extern int g_stub_a;
extern void *GetEventWindow(void);

#include "classes.h"
extern void PediaWin_Done(PediaWin *w);

void PediaWin_OnOKClick(void)
{
    char *w;
    char *cw;

    cw = (char *)GetEventWindow();
    w = cw != 0 ? cw - 0x26 : cw;
    if (w == 0)
        return;
    PediaWin_Done((PediaWin *)w);
}
