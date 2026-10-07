/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 35 of the imported file. */
/* PORT: this file's own declarations of dlg_control_init_1050_9418 removed: the header declares it from the definition (W1 step 3). */
/* checkbox_init: candidate for 1050:8718 (138 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Initialises a check-box control, and it is `listbox_init`'s opposite number: where that
 * one sizes from a caller-supplied rectangle and registers the MSListBox class, this asks
 * `text_row_rect` for the rectangle its LABEL needs and hands the result to
 * `MSInitCheck`.
 *
 * The kind passed to `dlg_control_init_1050_9418` is the literal 2, as the list box's is the
 * literal 7 -- each control family in this module has its own kind code.
 *
 * Three fields past the end of `DlgControl` (0x1a bytes) are set: a 32-bit zero at +0x1a
 * written with one `MOV DWORD`, then 1 at +0x20 and 0 at +0x1e, in that order and not in
 * address order.
 *
 * The text is a pointer at bp+0x14, not two ints: `text_row_rect` takes
 * `char *` first and `MSInitCheck` takes it third, and both agree.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern unsigned MSInitCheck(RECT *r, void *ctl, const char *text, int visible);

/* same object: 4-byte `push cs; call ` */

void checkbox_init(SCheckBox *checkbox, DialogWin *win, int id, int rx, int ry, char *text)
{
    RECT r;

    text_row_rect(text, rx, ry, &r);
    dlg_control_init_1050_9418((DlgControl *)(checkbox), win, 2, id, &r);
    checkbox->on_change = 0L;
    checkbox->enabled = 1;
    checkbox->checked = 0;
    checkbox->hwnd = MSInitCheck(&r, checkbox, text, 1);
}
