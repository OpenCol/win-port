/* EuropeWin_Done: candidate for 1048:8d5d (49 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_Done
 * name-source: sibling-build
 *
 * Closes the Europe window down: -1 into its `nation` at +0x77a -- no nation selected --
 * 0 into DGROUP:0xb4, and the focus
 * handed back to the main window at SEG20:0x5ca through `MSSetFocus_of_1048_e0cd`, which is
 * reached by the four-byte form and so is defined here as a stub (rule 47).
 *
 * `EuropeWin_Close` describes it as "EuropeWin_Done on the object owning the current window".
 *
 * RENAMED 2026-09-26 from europe_release_focus: the Mac build's EuropeWin::Done(void): this only, shutting the Europe window down -- no nation selected, the tutorial flag cleared, the focus given back. It sits in the Europe module (1048) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
class MainWin;
extern MainWin g_main_win;   /* SEG20:0x5ca */

extern int g_stub_a;

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void MSSetFocus_of_1048_e0cd(Screen *w);

void EuropeWin_Done(EuropeWin *o)
{
    o->nation = -1;
    g_europe_screen_open = 0;
    MSSetFocus_of_1048_e0cd((Screen *)&g_main_win);
}
