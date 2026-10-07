/* control_is_current: candidate for 1070:502b (101 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 101, not the census 94. The trailing 7 bytes are the dead second epilogue.
 *
 * Whether a window handle names the control its owner currently has selected: take the object
 * behind the HWND, follow the pointer at +2 to the control and the one at +0x14 to the
 * item, ask the control for its index at +0x2a, and compare that against the item's at
 * +0xa. `same_control_owner` is the next function and asks the other half of the
 * same question.
 *
 * Three locals and two of them are pointers followed out of the first, which is why the
 * object is re-read from its frame slot for each: `-Od` keeps nothing across the `LES`.
 *
 * Both pointers are loaded as one 32-bit `MOV` (`66 26 8B 47 nn`), which is a `` pointer
 * field under `-3`.
 *
 * Two exits, each with its own epilogue.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` */

extern int get_cur_item_1070_99ac(void *ctl);

int control_is_current(unsigned hwnd)
{
    CtrlBlock *o;
    void *ctl;
    RadioItem *item;

    o = object_from_hwnd(hwnd);
    ctl = o->ctl;
    item = (RadioItem *)o->data;
    if (get_cur_item_1070_99ac((SRadioSet *)ctl) == item->index)
        return 1;
    return 0;
}
