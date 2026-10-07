/* set_hpage: candidate for 1060:a017 (65 bytes).  Verdict: RELOCATABLE.
 * symbol: set_hpage
 *
 * Measures the control through scroll_get_range_ex_1060_adea (start and end through two
 * pointers) and stores v at +0x78 when the span is at least v.  The helper
 * is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from control_set_width_1060_a017: it stores its argument at EventWin+0x78 -- hpage, the horizontal scroll page, in every EventWin class declaration -- when the scroll range is at least that wide. Its twin at 1060:a058 stores +0x7a, vpage.
 */

#include "colonization.h"

#include "classes.h"
extern int g_stub_a;

void set_hpage(EventWin *o, int page)
{
    int lo;
    int hi;

    scroll_get_range_ex_1060_adea(o, &lo, &hi);
    if (hi - lo >= page)
        o->hpage = page;
}

