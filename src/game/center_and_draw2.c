/* center_and_draw2: candidate for 1040:a5b1 (119 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: center_and_draw2
 *
 * Centres an item of width return_zero_1040_a4cb(w) in a span (x, width), clamped
 * to 0 through max_ptr -- whose first parameter is a C++ const reference,
 * so the literal 0 becomes a stack temporary bound at the call -- and
 * draws it through return_arg2_1040_a587.  The module is C++; the two helpers are
 * -called and defined in-file as stubs.
 */

#include "colonization.h"

extern int g_stub_a;

extern "C" int center_and_draw2(void *w, int x, int width, int c, int d, int e)
{
    int cx;
    int zero;

    cx = (width >> 1) + x - (return_zero_1040_a4cb(w) >> 1);
    cx = *max_ptr_1040_b582((zero = 0, &zero), &cx);
    cx = return_arg2_1040_a587(w, cx, c, d, e);
    return cx;
}
