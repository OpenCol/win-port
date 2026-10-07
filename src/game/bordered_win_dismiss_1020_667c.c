/* bordered_win_dismiss_1020_667c: candidate for 1020:667c (34 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Raises the flag at DGROUP:0x4bec and ends the modal loop of the window whose port is SEG22:0x30.
 *
 * ITS TWIN AT 1020:665a IS THE SAME FUNCTION, instruction for instruction -- same flag, same
 * port, same call. Two entry points with identical bodies is what a source file with two
 * names for one action looks like, and the byte match is what says they really are identical
 * rather than nearly so.
 *
 * `g_4bec` IS WRITTEN AND NEVER READ. Three instructions in the image touch 0x4bec -- the
 * `mov word [0x4bec],0x0` in `PopupBoxDraw` and the two `mov word [0x4bec],0x1` in the
 * twin refresh functions -- and nothing anywhere loads it. So the flag records that the window
 * was dismissed and no one asks. It is vestigial, which is why it is not named for the question
 * it would have answered; see `tools/check_dead_dgroup.py`.
 *
 * RENAMED 2026-09-26 from refresh_port_30_1020_667c: one of the two identical dismiss handlers PopupBoxDraw installs on its window
 * (left button, right button, key): sets g_4bec -- cleared by PopupBoxDraw, read by nothing,
 * so it keeps its address name -- and ends the window's modal loop. It
 * refreshes nothing; the old name read EventWin_EndModal as a port refresh.
 */

#include "colonization.h"

#include "classes.h"

extern void EventWin_EndModal(void *port);

void bordered_win_dismiss_1020_667c(void)
{
    g_4bec = 1;
    EventWin_EndModal((EventWin *)((char *)&g_win + 0x26));
}
