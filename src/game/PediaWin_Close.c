/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* PediaWin_Close: candidate for 1060:2bc3 (71 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Looks up the current window's owner and returns 1 -- a handler whose body was reduced to
 * the owner lookup.
 *
 * RENAMED 2026-09-27 from cur_win_ret1: BEYOND THE PROPOSALS; 2a47 vacates the name first. Mac PediaWin::Close(void), CODE 20 0x167a, 50 bytes, decoded: GetEventWindow; if non-null subtract 0x2c; store it in a local; moveq #1,d0; return. 2bc3, int cur_win_ret1(void), 71 bytes: w = GetEventWindow(); o = w ? w - 0x26 : w; return 1. The same body, the window-to-object offset aside. Neither has a direct caller; both are installed as the close handler. Layout: directly after OnOKClick, before HandleLeftClick, on both.
 */

#include "colonization.h"

extern void *GetEventWindow(void);

int PediaWin_Close(void)
{
    char *o;
    char *w = (char *)GetEventWindow();

    o = w != 0 ? w - PORT_DIALOG_BASE : w;
    return 1;
}
