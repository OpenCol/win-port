/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* PediaWin_LeftMouseClick: candidate for 1060:3153 (169 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A click on the Colonopedia. The window is `GetEventWindow()` converted down by 0x26 (the
 * base-class conversion of docs/findings/cpp-base-offset.md), it always takes the focus, and
 * the click is dispatched only when the point is inside the window's rectangle at
 * +0x76a..+0x770. `PediaWin_OnScroll` is the next function.
 *
 * `win_take_focus` IS CALLED IN BOTH BRANCHES, and that is the shipped code rather than a
 * reading of it: `-Od` never merges identical call tails, which is the second witness this
 * corpus has for that flag. Hoisting it above the `if` -- which is what the behaviour says --
 * removes one of the two calls and thirteen bytes.
 *
 * The test is written as the OUTSIDE case first, four `||` operands, with an early `return`
 * and no `else`. The `return` is what the bytes insist on: the first branch ends with its own
 * epilogue where an `else` would end with a `JMP` over the second (rule 43, from the other
 * side).
 *
 * The two locals are declared derived-then-window: Borland gives the first-declared the
 * higher slot and the shipped keeps the raw pointer at `[bp-0xa]`.
 *
 * RENAMED 2026-09-26 from pedia_mouse: the Mac build's PediaWin::LeftMouseClick(short, short): open_colonopedia (now PediaWin_Show) installs it as the window's lbutton_down handler, and it takes the same two shorts.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);   /* 1060:9fcd */

/* same object: 4-byte `push cs; call ` */

extern void PediaWin_HandleLeftClick(PediaWin *p, int x, int y);

void PediaWin_LeftMouseClick(register int x, register int y)
{
    PediaWin *p;
    void *w;

    w = GetEventWindow();
    p = (PediaWin *)(w != 0 ? (char *)w - PORT_DIALOG_BASE : (char *)w);
    if (p->rect.left > x || p->rect.right < x || p->rect.top > y || p->rect.bottom < y) {
        win_take_focus((DialogWin *)((char *)p + PORT_DIALOG_BASE));
        return;
    }
    win_take_focus((DialogWin *)((char *)p + PORT_DIALOG_BASE));
    PediaWin_HandleLeftClick(p, x, y);
}
