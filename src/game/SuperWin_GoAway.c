/* SuperWin_GoAway: candidate for 1050:6ca0 (107 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Takes down an event dialog and puts the screen back: hide the dialog's own window at +0x26,
 * restore palette entries 10 .. 245 of `g_game_palette` (SEG20:0x1ac) from the copy the dialog
 * saved at +0x150, let pending messages run, then hide and detach the sub-window at +0xc4 and
 * let messages run again. `declaration_of_independence` and `show_fathers` call it on their stack
 * SuperWin when they are done with it.
 *
 * The palette range is the 236 entries a dialog is allowed to change -- the first and last ten
 * are the system colours Windows keeps -- so the saved copy is exactly what the dialog could
 * have disturbed.
 *
 * `MSShowMenuBar` and CTable_Fill are calls, so extern. MSHideWindow_of_1050_8cd8,
 * MSDoAllTasks_1050_8be7 and dialog_dispose_window_1050_906a are this segment's, reached by the four-byte
 * `push cs; call `, so they are defined here (rule 47).
 *
 * RENAMED 2026-09-27 from dialog_destroy: Mac SuperWin::GoAway(void). Callers: declaration_of_independence, show_fathers and woodcut_show, 3 of 3 on both. Callees: MSShowMenuBar, MSHideWindow x2 (MSHideWindow_of x2), CTable::Fill, MSDoAllTasks x2 and MSCloseWindow (dialog_dispose_window), the same on both. Layout: Mac SuperWin RightMouseDown, KeyDown, Execute, GoAway; Windows SuperWin_RightMouseDown, KeyDown, Execute, 6ca0, one for one. Arity: this. Size: at most 146 (the next function starts at 6d32), against 180.
 */

#include "colonization.h"

#include "classes.h"

extern void MSShowMenuBar(void);
struct logpal { char pad[1]; };
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void MSHideWindow_of_1050_8cd8(DialogWin *w);

extern void dialog_dispose_window_1050_906a(DialogWin *sub);

void SuperWin_GoAway(SuperWin *w)
{
    MSShowMenuBar();
    MSHideWindow_of_1050_8cd8((DialogWin *)((char *)w + 0x26));
    CTable_Fill(&g_game_palette, 0xa, 0xec, (char *)w + 0x150);
    MSDoAllTasks_1050_8be7();
    MSHideWindow_of_1050_8cd8((DialogWin *)((char *)w + 0xc4));
    dialog_dispose_window_1050_906a((DialogWin *)((char *)w + 0xc4));
    MSDoAllTasks_1050_8be7();
}
