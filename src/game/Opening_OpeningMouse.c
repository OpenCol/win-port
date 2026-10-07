/* Opening_OpeningMouse: candidate for 1010:1e79 (92 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Opening_OpeningMouse
 * name-source: sibling-build
 *
 * A click during the opening (the Opening at DGROUP:0x964): nothing while
 * its state (+0x550) is -1, state 4 marks the opening DONE (+0x1e76), any other
 * state asks to SKIP the current part (+0x56a, which `Opening_TimerProc` reads on the next tick and
 * answers by jumping to state 3) and runs Opening_RefreshUpdatePort (-called,
 * defined in-file as a stub).
 *
 * RENAMED 2026-09-26 from opening_click_1010_1e79: the Mac build's Opening::OpeningMouse(short, short): Opening_ctor installs it on lbutton_down, left click and right click. The body reads no argument, so the Windows prototype has none.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"
extern Opening *g_opening;             /* DGROUP:0x964 */
extern int g_stub_a;

extern "C" void Opening_RefreshUpdatePort(Opening *w);

void Opening_OpeningMouse(void)
{
    Opening *w = g_opening;

    if (w->state == -1)
        return;
    else if (w->state == 4)
        w->sequence_done = 1;
    else {
        w->skip = 1;
        Opening_RefreshUpdatePort(w);
    }
}

