/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* hide_owner_window: candidate for 1050:6ec7 (90 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `fame_OnLMouseDown`'s sibling: step back from the current window to the object that
 * owns it and hand that object to `win_hide_1050_6e73`. The `- 0x26` is the same step back every
 * handler in this module makes, and it is guarded -- a null window is not decremented.
 *
 * THE OWNER IS TESTED TOO. Where the sibling calls unconditionally, this one checks the
 * result of the subtraction against zero as well, which is only ever false when the window
 * was null. The redundant test is in the shipped code and is what the two locals are for.
 *
 * 90 bytes, not the census 83: the written-out `return 0;` adds a dead second epilogue, and
 * this frame's epilogue is seven bytes (rule 44).
 *
 * RENAMED 2026-09-26 from owner_win_6e73: win_hide on the current window's owner, when there is one --
 * fame_OnLMouseDown's sibling.
 */

#include "colonization.h"

#include "w16types.h"
extern void *GetEventWindow(void);

#include "records.h"
/* same object: 4-byte `push cs; call ` */
extern void win_hide_1050_6e73(BackdropWin *o);

int hide_owner_window(void)
{
    char *o;
    char *w;

    w = (char *)GetEventWindow();
    o = w != 0 ? w - PORT_DIALOG_BASE : w;
    if (o != 0)
        win_hide_1050_6e73((BackdropWin *)o);
    return 0;
}
