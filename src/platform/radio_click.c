/* radio_click: candidate for 1070:434d (304 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: radio_click
 * name-source: descriptive
 *
 * A click on a radio group: take the focus, move the control's selection to the model's, and
 * repaint both the item that was selected and the one that now is -- the SAME expression twice
 * with `set_cur_item_1070_9994` between them, so the first invalidate names the old item and the
 * second the new one. Then publish the control at DGROUP:0x2ae0 and call handler slot 26.
 *
 * Nothing happens unless the two disagree AND the model's word at +0xc is exactly 1, so a
 * disabled group swallows the click before anything repaints.
 *
 * The item array is 0x92 bytes to the entry and its first word is the item's HWND, which is
 * all this function reads of it. `PUSH AX / PUSH DX ... POP ES / POP BX` around the index call
 * is Borland spilling the base pointer across it, not a different addressing mode.
 *
 * Arguments to the handler are evaluated right to left, which is why `get_cur_item_1070_99ac` (+0x2a) is called
 * before `ctl_id_1070_96c5` (+0x2) even though it is the later argument.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"
    /* +0xa, +0xc */

/* same object: 4-byte `push cs; call ` */

extern "C" int get_cur_item_1070_99ac(SRadioSet *ctl);
extern "C" RadioItem *get_items_1070_99c5(SRadioSet *radios);
extern "C" void set_cur_item_1070_9994(SRadioSet *radios, int cur_item);
extern "C" void *ctl_window_1070_96a8(SRadioSet *ctl);
extern "C" int ctl_id_1070_96c5(SRadioSet *ctl);
extern "C" void call_radio_changed(SRadioSet *ctl, int id, int item);

void radio_click(register unsigned hwnd)
{
    CtrlBlock *o;
    SRadioSet *ctl;
    Model *m;

    o = object_from_hwnd(hwnd);
    if (o == 0)
        return;
    ctl = (SRadioSet *)o->ctl;
    m = (Model *)o->data;
    SetFocus(hwnd);
    if (get_cur_item_1070_99ac(ctl) != m->sel && m->enabled == 1) {
        InvalidateRect(get_items_1070_99c5(ctl)[get_cur_item_1070_99ac(ctl)].hwnd, 0L, 0);
        set_cur_item_1070_9994(ctl, m->sel);
        InvalidateRect(get_items_1070_99c5(ctl)[get_cur_item_1070_99ac(ctl)].hwnd, 0L, 0);
    }
    g_current_window = ctl_window_1070_96a8(ctl);
    call_radio_changed(ctl, ctl_id_1070_96c5(ctl), get_cur_item_1070_99ac(ctl));
}
