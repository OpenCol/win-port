/* ClosingWin_Keys: candidate for 1010:0403 (20 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * ClosingWin's keydown handler: sets the closing flag (DGROUP:0x4b98).
 *
 * RENAMED 2026-09-26 from closing_flag_set_1010_0403: the Mac build's ClosingWin::Keys(short): ClosingWin_ctor installs it as the keydown handler (twice). The body reads no argument, so the Windows prototype has none. The width matches.
 */

#include "colonization.h"

void ClosingWin_Keys(void)
{
    g_closing_flag = 1;
}
