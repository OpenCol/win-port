/* sregion_init_1048_dfb0: candidate for 1048:dfb0 (131 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: sregion_init_1048_dfb0
 * symbol: sregion_init_1048_dfb0
 *
 * This module's own copy of the region-control initialiser: dlg_control_init with
 * kind 1, the five longs at +0x1a..+0x2a cleared, MSInitRegion for the window handle.
 * dlg_control_init is -called and defined in-file. One of three per-module copies.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void DialogWin_AddControl(void *win, DlgControl *ctl);
extern short MSInitRegion(RECT *r, DlgControl *ctl);

extern "C" void dlg_control_init_1048_e633(DlgControl *ctl, void *win, int kind, int id, RECT *r);

void sregion_init_1048_dfb0(SRegion *o, void *win, int id, RECT *r)
{
    dlg_control_init_1048_e633(o, win, 1, id, r);
    o->on_enter = 0;
    o->on_leave = 0;
    o->on_click = 0;
    o->on_press = 0;
    o->on_dblclk = 0;
    o->hwnd = MSInitRegion(r, o);
}
