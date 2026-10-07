/* PORT: this file's own declarations of DialogWin_AddControl removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of MSInitRegion removed: the header declares it from the definition (W1 step 3). */
/* sregion_init_1010_e8bc: candidate for 1010:e8bc (131 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: sregion_init_1010_e8bc
 *
 * Builds a region control: dlg_control_init with kind 1, the five longs at
 * +0x1a..+0x2a cleared, MSInitRegion for the window handle.
 * dlg_control_init is -called and defined in-file.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern "C" void dlg_control_init_1010_edf9(DlgControl *ctl, void *win, int kind, int id, RECT *r);

void sregion_init_1010_e8bc(SRegion *o, void *win, int id, RECT *r)
{
    dlg_control_init_1010_edf9(o, win, 1, id, r);
    o->on_enter = 0;
    o->on_leave = 0;
    o->on_click = 0;
    o->on_press = 0;
    o->on_dblclk = 0;
    o->hwnd = MSInitRegion(r, o);
}
