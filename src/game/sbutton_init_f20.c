/* sbutton_init_f20: candidate for 1010:eabb (58 bytes).  Verdict: RELOCATABLE.
 * symbol: sbutton_init_f20
 *
 * Builds a button and records one more word at +0x20 of it, the face colour `MSInitButton` paints, which the shared initialiser does not take. The initialiser and its own callee are -called and defined in-file.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void DialogWin_AddControl(void *win, DlgControl *ctl);
extern short MSInitButton(RECT *rgn, SButton *obj, char *title, int visible);

extern void dlg_control_init_1010_edf9(DlgControl *ctl, void *win, int kind, int id, RECT *r);

extern void sbutton_init_1010_eeac(SButton *button, void *win, int id, RECT *r, char *title);

void sbutton_init_f20(SButton *button, void *win, int id, RECT *r, int colour, char *title)
{
    button->colour = colour;
    sbutton_init_1010_eeac(button, win, id, r, title);
}
