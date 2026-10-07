/* ColonyWin_TimerProc: candidate for 1010:adfa (70 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_TimerProc
 * name-source: sibling-build
 *
 * Flips the flag at DGROUP:0xa06 and hides (flag now 0) or shows and repaints (flag 1) the
 * control 0x972 into the object at DGROUP:0xa00. The two show/hide wrappers are defined here
 * because the shipped calls to them are the compiler's own `push cs / call `.
 *
 * RENAMED 2026-09-26 from toggle_colony_ctl: the Mac build's ColonyWin::TimerProc(short, short): the callback ColonyWin_Show hands start_timer (700 ms, repeating). It reads neither timer argument, so the Windows prototype has none. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern ColonyWin *g_colony_window_ptr;   /* DGROUP:0xa00 */

extern "C" void MSHideControl_1010_e54d(DlgControl *control);

extern "C" void MSShowControl_1010_e574(DlgControl *control);

void ColonyWin_TimerProc(void)
{
    g_colony_ctl_visible = !g_colony_ctl_visible;
    if (g_colony_ctl_visible == 0)
        MSHideControl_1010_e54d(&g_colony_window_ptr->b[1]);
    else
        MSShowControl_1010_e574(&g_colony_window_ptr->b[1]);
}
