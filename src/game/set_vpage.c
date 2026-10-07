/* set_vpage: candidate for 1060:a058 (65 bytes).  Verdict: RELOCATABLE.
 * symbol: set_vpage
 *
 * Measures the control through scroll_get_range_ex_1060_ae1d (start and end through two
 * pointers) and stores v at +0x7a when the span is at least v.  The helper
 * is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from control_set_width_1060_a058: it stores its argument at EventWin+0x7a -- vpage, the vertical scroll page, in every EventWin class declaration -- when the scroll range is at least that tall. Its twin at 1060:a017 stores +0x78, hpage.
 */

#include "colonization.h"

#include "classes.h"
extern int g_stub_a;

void set_vpage(EventWin *o, int page)
{
    int start;
    int end;

    scroll_get_range_ex_1060_ae1d(o, &start, &end);
    if (end - start >= page)
        o->vpage = page;
}

