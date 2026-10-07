/* SRadioSet_Show: candidate for 1060:915b (82 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Shows and invalidates the window of every item (0x92-byte records at
 * the pointer at +0x2e, count at +0x26).
 *
 * RENAMED 2026-09-27 from list_show_items: the Mac build's SRadioSet_Show, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Shows and invalidates each item's window (two distinct calls per item, like the Mac's two callees); called only by Popup_draw, the Mac Show's only caller Popup::draw; this + void, ratio 1.17.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

void SRadioSet_Show(SRadioSet *o)
{
    register int i;

    for (i = 0; o->count > i; i++) {
        show_hwnd(((ListItem *)o->items)[i].hwnd);
        invalidate_hwnd(((ListItem *)o->items)[i].hwnd);
    }
}
