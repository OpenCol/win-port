/* SuperWin_LeftMouseDown: candidate for 1050:6bfb (27 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * One of three button handlers of SuperWin_Init's dialog: sets the flag at DGROUP:0x2520
 * unless the super window is open (DGROUP:0x251e).
 *
 * RENAMED 2026-09-26 from mark_2520_no_super_1050_6bfb: one of the three dismiss handlers SuperWin_Init installs (left click,
 * right click, key): sets g_modal_done unless the super window is open.
 *
 * RENAMED 2026-09-26 from super_win_dismiss_1050_6bfb: the Mac build's SuperWin::LeftMouseDown(short, short): SuperWin_Init installs it as the left-click handler. The body reads no argument, so the Windows prototype has none.
 */

#include "colonization.h"

void SuperWin_LeftMouseDown(void)
{
    if (g_super_win_open == 0)
        g_modal_done = 1;
}
