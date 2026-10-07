/* sbutton_init_1058_c11b: candidate for 1058:c11b (100 bytes).  Verdict: RELOCATABLE.
 *
 * Control initialisation (kind 6) through dlg_control_init_1058_c3f6, then the Windows button through
 * MSInitButton; the handle lands at +0x10, the long at +0x1c is cleared and the word at
 * +0x1a set to 1.
 *
 * The trailing four bytes are the button's TITLE, not a width/height pair. Both totals are
 * four, so the comparator cannot tell them apart (§119); `EuropeWin_Init` and
 * `ReportWin_TradeRoute2` settle it -- they pass "~Recruit", "Close" and the like, and the
 * Mac symbol table spells the callee `MSInitButton`.
 *
 * RENAMED 2026-09-26 from SButton_init: this module's copy of the SButton initialiser, the same body as
 * sbutton_init_1010_eeac: a kind-6 DlgControl, action cleared, MSInitButton for the handle, enabled 1.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void dlg_control_init_1058_c3f6(SButton *ctl, void *win, int kind, int id, RECT *pos);
extern short MSInitButton(RECT *rgn, SButton *obj, char *title, int style);

void sbutton_init_1058_c11b(SButton *button, void *win, int id, RECT *pos, char *title)
{
    dlg_control_init_1058_c3f6(button, win, 6, id, pos);
    button->action = 0;
    button->hwnd = MSInitButton(pos, button, title, 1);
    button->enabled = 1;
}
