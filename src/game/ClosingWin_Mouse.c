/* ClosingWin_Mouse: candidate for 1010:03ef (20 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * ClosingWin's first button handler: sets the closing flag (DGROUP:0x4b98).
 *
 * RENAMED 2026-09-26 from closing_flag_set_1010_03ef: the Mac build's ClosingWin::Mouse(short, short): ClosingWin_ctor installs it as the lbutton_down handler (twice). The body reads no argument, so the Windows prototype has none. The width matches.
 */

#include "colonization.h"

void ClosingWin_Mouse(void)
{
    g_closing_flag = 1;
}
