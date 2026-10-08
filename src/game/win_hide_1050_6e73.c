/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* win_hide_1050_6e73: candidate for 1050:6e73 (84 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Hides a dialog: the window at +0x26 is hidden, the port is rebuilt over a 1x1 rectangle at the origin
 * through `GameWin_ResizePort`, and `shown_half` at +0xc4 is set to -1, the "hidden" value
 * `show_backdrop` tests before it grows the port back and shows the window.
 *
 * The first three steps are the same three `PediaWin_Done` takes for a non-modal
 * window, and the -1 goes to +0xc4 where that one stores it at +0x77a -- in both, the first
 * member after the base class (GameWin here, ColChild there). So the two are the same close on two
 * different window layouts, and this module's object is the smaller one.
 *
 * `SetRect(&r, 0, 0, 1, 1)` is the new size: `GameWin_ResizePort` re-initialises the port
 * over it (GRPort_Init) and sets the window's client size to 1 x 1; nothing is copied.
 *
 * `MSHideWindow_of_1050_8cd8` is -called and is a stub here (rule 47); the other two are
 * calls.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

#include "classes.h"
extern void GameWin_ResizePort(GameWin *w, void *rect);

/* same object: 4-byte `push cs; call ` */
extern void MSHideWindow_of_1050_8cd8(DialogWin *sub);

void win_hide_1050_6e73(BackdropWin *w)
{
    RECT r;

    SetRect(&r, 0, 0, 1, 1);
    MSHideWindow_of_1050_8cd8((DialogWin *)((char *)w + PORT_DIALOG_BASE));
    GameWin_ResizePort((GameWin *)w, &r);
    w->shown_half = -1;
}
