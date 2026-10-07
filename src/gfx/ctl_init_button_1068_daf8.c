/* ctl_init_button_1068_daf8: candidate for 1068:daf8 (100 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -r-
 * symbol: ctl_init_button_1068_daf8
 *
 * Lays a button control out through this module's own DlgControl initialiser with kind 6,
 * clears the long at +0x1c, creates the button over the same rectangle and marks the
 * control live. One of four per-module copies.
 *
 * The trailing four bytes are the button's TITLE, not a width/height pair. Both totals are
 * four, so the comparator cannot tell them apart (§119); `EuropeWin_Init` and
 * `ReportWin_TradeRoute2` settle it -- they pass "~Recruit", "Close" and the like, and the
 * Mac symbol table spells the callee `MSInitButton`.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void dlg_control_init_1068_e478(SButton *ctl, void *win, int kind, int id, RECT *r);
extern short MSInitButton(RECT *rgn, SButton *obj, char *title, int style);
void ctl_init_button_1068_daf8(SButton *button, DialogWin *win, int id, RECT *r, char *title)
{
    dlg_control_init_1068_e478(button, win, 6, id, r);
    button->action = 0;
    button->hwnd = MSInitButton(r, button, title, 1);
    button->enabled = 1;
}
