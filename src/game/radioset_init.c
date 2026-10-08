/* PORT: the count field is 16-bit: passed as i16 * (W4). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 39 of the imported file. */
/* PORT: this file's own declarations of dlg_control_init_1050_9418 removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of build_radio_buttons removed: the header declares it from the definition (W1 step 3). */
/* radioset_init: candidate for 1050:881c (186 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Initialises the popup's RADIO SET -- `Popup_make_controls` builds `new SRadioSet` and passes
 * it here, then sets its double-click handler and current item. The helper it calls,
 * `build_radio_buttons`, loads THREE bitmaps per item through `LoadBitmap` and
 * allocates a handle; that is what made this read as an image button at first.
 *
 * Its control kind is 3, where `checkbox_init` passes 2 and `listbox_init` 7.
 *
 * Six fields past the end of `DlgControl` are set before the helper runs -- three 32-bit
 * zeros at +0x1a, +0x1e and +0x22, then the two size words at +0x26 and +0x28, then a zero at
 * +0x2a. The two size words go in SWAPPED: the argument at bp+0x16 lands at +0x26 and the one
 * at bp+0x14 at +0x28.
 *
 * The handle comes back at +0x2c and `MSHLock` turns it into the pointer at +0x2e/+0x30,
 * so the control keeps both and never asks again.
 *
 * The helper's first argument is `&c->count`, the size pair, not the control's rectangle. I
 * had it as `&c->rect` on the first compile because I counted `RECT` as sixteen bytes; it is
 * four `int`s and eight, which is also what puts the handle at +0x10 rather than +0x18 in
 * every control in this family.
 *
 * RENAMED 2026-09-26 from imagebutton_init: its one caller, Popup_make_controls, does p->radios = new SRadioSet and passes that object here, then sets the radio set's double-click handler and current item on it: this initialises the popup's RADIO SET (its items drawn from three bitmaps each, which is what made it read as an image button). Named after its siblings checkbox_init and listbox_init; the Mac SRadioSet class has no Init to borrow.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` */

void radioset_init(SRadioSet *radios, DialogWin *win, int id, RECT *r,
                          register int cols, register int count, char * *names)
{
    dlg_control_init_1050_9418((DlgControl *)(radios), win, 3, id, r);
    radios->on_change = 0L;
    radios->on_dblclk = 0L;
    radios->on_rbutton_down = 0L;
    radios->count = count;
    radios->cols = cols;
    radios->cur_item = 0;
    radios->hmem = build_radio_buttons((i16 *)&radios->count, r, cols, radios, count, names, 1);
    radios->items = MSHLock(radios->hmem);
}
