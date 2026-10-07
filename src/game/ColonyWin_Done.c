/* PORT: this file's own declarations of MSHideWindow_of_1010_eb6e removed: the header declares it from the definition (W1 step 3). */
/* ColonyWin_Done: candidate for 1010:b004 (54 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_Done
 * name-source: sibling-build
 *
 * Hides the window (its DialogWin at +0x26), sets its colony (+0xa12) to -1 and gives the
 * focus back to the main window.
 *
 * RENAMED 2026-09-26 from win_close_and_reset: the Mac build's ColonyWin::Done(void): this only; it hides the colony window, sets its colony to -1 and gives the focus back to the main window -- what EuropeWin_Done does for Europe -- and ColonyWin_Close calls it. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;   /* SEG20:0x05ca */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void MSSetFocus_of_1010_eb8f(void *p);
void ColonyWin_Done(ColonyWin *w)
{
    MSHideWindow_of_1010_eb6e(&(DialogWin &)*w);
    w->colony = -1;
    MSSetFocus_of_1010_eb8f((Screen *)&g_main_win);
}
