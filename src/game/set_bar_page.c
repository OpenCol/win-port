/* set_bar_page: candidate for 1060:90da (65 bytes).  Verdict: RELOCATABLE.
 * symbol: set_bar_page
 *
 * Measures the control through ctl_get_scroll_range (start and end through two
 * pointers) and stores v at +0x22 when the span is at least v.  The helper
 * is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-27 from control_set_width_1060_90da: it stores the scroll bar's PAGE at SScrollBar +0x22 (the value get_bar_page reads) when the range's span is at least that big -- the same shape as set_hpage/set_vpage, which left the control_set_width_* name for the same reason.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;

extern void ctl_get_scroll_range(void *control, int *pa, int *pb);

void set_bar_page(SScrollBar *bar, int page)
{
    int lo;
    int hi;

    ctl_get_scroll_range((DlgControl *)bar, &lo, &hi);
    if (hi - lo >= page)
        bar->page = page;
}

