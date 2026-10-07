/* ColChild_ResizeCWin: candidate for 1048:06e0 (125 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_ResizeCWin
 * name-source: sibling-build
 *
 * Lays a window out from a content size: the width grows by 0x14 and the height by 0x2a to
 * make room for the frame, a 0x20 x 0x16 rectangle is placed at the top right and handed to
 * `ctl_move` through the object at +0x6ea, and the client rectangle at +0x76a is inset
 * by 10 on the left, right and bottom, with its top at 0x20.
 *
 * +0x76a IS A RECT, not a pair of words. `EuropeWin_EuropeShowBig` reads +0x76a and +0x76c
 * as the window's x and y origin; this writes all four words with one `SetRect`, so those two
 * are the rect's left and top.
 *
 * `SetRect` is ``, so its five arguments go left to right in push order -- which is what
 * makes the first rect `(si - 0x20, 0, si, 0x16)` and not the other way round.
 *
 * THE PARAMETERS THEMSELVES ARE THE REGISTERS. The shipped loads BOTH into SI and DI at
 * entry and only then grows them -- `MOV SI,[bp+0xa] / MOV DI,[bp+0xc] / LEA AX,[si+0x14]
 * / MOV SI,AX` -- which is `register int cx, register int cy` reassigned in place. With
 * separate locals Borland loads the parameter, adds, and stores, and never keeps the
 * original in a register at all.
 *
 * `cx = cx + 0x14` and not `cx += 0x14`: the compound form gives `ADD SI,0x14` in three
 * bytes where the shipped spends five on `LEA` and a move.
 *
 * RENAMED 2026-09-26 from set_window_layout: the Mac build's ColChild::ResizeCWin(short, short): this plus a content width and height, the window resized around them. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void win_clamp_client_size(ColChild *w, int cx, int cy);

/* same object: 4-byte `push cs; call ` */
extern "C" void ctl_move(void *control, int x, int y);

void ColChild_ResizeCWin(ColChild *w, register int cx, register int cy)
{
    RECT r;

    cx = cx + 0x14;
    cy = cy + 0x2a;
    SetRect(&r, cx - 0x20, 0, cx, 0x16);
    ctl_move(&w->btn, r.left, r.top);
    SetRect(&w->rect, 0xa, 0x20, cx - 0xa, cy - 0xa);
    win_clamp_client_size(w, cx, cy);
}
