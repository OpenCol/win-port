/* radio_init: candidate for 1068:df8f (173 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Lays a radio group out: this module's own DlgControl initialiser with kind 3 -- where
 * `ctl_init_button_1068_daf8` passes 6 -- then the three callbacks cleared, the COLUMN count
 * stored at +0x28, the current item at +0x2a set to 0, and `MSInitRadio` over the same
 * rectangle. The handle it returns is kept at +0x2c and LOCKED immediately; the locked pointer
 * at +0x2e is the items array get_items returns.
 *
 * The lock is the interesting part: a radio group holds both the handle and the pointer into
 * it, side by side, so nothing has to lock it again to read a button state. That is the
 * `MSHLock` whose result goes straight into the record rather than into a local.
 *
 * `MSInitRadio` (1070:3dd1) is `(int *out, RECT *area, int cols,
 * void *owner, char *captions, int visible)`: it splits the underscore-separated
 * captions, builds that many buttons in `cols` columns and writes how many through `out`. So
 * +0x26 is the BUTTON count, filled by MSInitRadio -- the word radio_count reads -- and the
 * parameter stored at +0x28 is the column count. (Corrected 2026-09-26: this header had read
 * +0x28 as the button count and +0x26 as a sub-object handed over.)
 *
 * The column count is the only parameter that gets a register (SI), because it is used twice:
 * once stored and once passed on.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void dlg_control_init_1068_e478(SRadioSet *ctl, void *win, int kind, int id, RECT *r);
extern short MSInitRadio(int *out, RECT *r, int cols, SRadioSet *owner, char *captions, int visible);

void radio_init(SRadioSet *radio, DialogWin *win, int id, RECT *r,
                              register int cols, char *captions)
{
    dlg_control_init_1068_e478(radio, win, 3, id, r);
    radio->on_change = 0;
    radio->on_dblclk = 0;
    radio->on_rbutton_down = 0;
    radio->cols = cols;
    radio->cur_item = 0;
    radio->hmem = MSInitRadio((int *)&radio->count, r, cols, radio, captions, 1);
    radio->items = MSHLock(radio->hmem);
}
