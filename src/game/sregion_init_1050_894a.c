/* PORT: this file's own declarations of DialogWin_AddControl removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of MSInitRegion removed: the header declares it from the definition (W1 step 3). */
/* sregion_init_1050_894a: candidate for 1050:894a (131 bytes).  Verdict: RELOCATABLE.
 * symbol: sregion_init_1050_894a
 *
 * This module's own copy of the region-control initialiser: dlg_control_init with
 * kind 1, the five longs at +0x1a..+0x2a cleared, MSInitRegion for the window handle.
 * dlg_control_init is -called and defined in-file. One of three per-module copies.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void dlg_control_init_1050_9418(DlgControl *ctl, void *win, int kind, int id, RECT *r);

void sregion_init_1050_894a(SRegion *o, DialogWin *win, int id, RECT *r)
{
    dlg_control_init_1050_9418((DlgControl *)o, win, 1, id, r);
    o->on_enter = 0;
    o->on_leave = 0;
    o->on_click = 0;
    o->on_press = 0;
    o->on_dblclk = 0;
    o->hwnd = MSInitRegion(r, (DlgControl *)o);
}
