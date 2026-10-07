/* AboutWin_Mouse: candidate for 1060:7eed (35 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Raises the counter at DGROUP:0x4e66 to 16 while it is under 15, and once it
 * is not, raises the flag at DGROUP:0x4e64 instead.
 *
 * The second copy of `AboutWin_Mouse`, byte for byte. Two identical functions
 * 0x23 bytes apart, which is what a default argument or an inline in two places looks like
 * after compilation.
 *
 * PRESS ONCE TO SKIP, TWICE TO CLOSE. While `g_about_step` is under 15 it jumps the
 * animation to 16, its last frame; after that it sets `g_about_done`, and `AboutWin_DoCredits`'s
 * `while (g_about_done == 0) AboutWin_UpdateCredits(w);` falls out.
 *
 * RENAMED 2026-09-26 from about_skip_or_close_1060_7eed: the Mac build's AboutWin::Mouse(short, short): AboutWin_ctor installs it as the lbutton_down handler. The body reads no argument, so the Windows prototype has none.
 */

#include "colonization.h"

void AboutWin_Mouse(void)
{
    if (g_about_step < 0xf)
        g_about_step = 0x10;
    else
        g_about_done = 1;
}
