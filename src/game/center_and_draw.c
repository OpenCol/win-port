/* center_and_draw: candidate for 1040:a51e (84 bytes).  Verdict: RELOCATABLE.
 * symbol: center_and_draw
 *
 * Centres an item of width return_zero_1040_a4b7(w) in a span (x, width), never
 * left of 0, and draws it through write_in_color.  The three helpers are
 * -called and defined in-file (max_int real, the others stubs).
 */

#include "colonization.h"

extern int g_stub_a;

extern int write_in_color(void *w, int x, int d, int e);

int center_and_draw(void *w, int x, int width, int y, int colour)
{
    register int cx;

    cx = (width >> 1) + x - (return_zero_1040_a4b7(w) >> 1);
    cx = max_int_1040_aab7(0, cx);
    cx = write_in_color(w, cx, y, colour);
    return cx;
}
