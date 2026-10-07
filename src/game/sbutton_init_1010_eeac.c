/* PORT: this file's own declarations of DialogWin_AddControl, MSInitButton removed: the header declares them from the definition (W1 step 3). */
/* sbutton_init_1010_eeac: candidate for 1010:eeac (100 bytes).  Verdict: RELOCATABLE.
 * symbol: sbutton_init_1010_eeac
 *
 * Builds a button control: dlg_control_init with kind 6, the long at +0x1c
 * cleared, MSInitButton for the window handle, the word at +0x1a set to 1.
 * dlg_control_init is -called and defined in-file.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void dlg_control_init_1010_edf9(DlgControl *ctl, void *win, int kind, int id, RECT *r);

void sbutton_init_1010_eeac(SButton *button, void *win, int id, RECT *r, char *title)
{
    dlg_control_init_1010_edf9((DlgControl *)button, win, 6, id, r);
    button->action = 0;
    button->hwnd = MSInitButton(r, button, title, 1);
    button->enabled = 1;
}
