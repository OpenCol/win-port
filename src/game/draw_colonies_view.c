/* draw_colonies_view: candidate for 1040:0b2f (61 bytes).  Verdict: RELOCATABLE.
 * symbol: draw_colonies_view
 *
 * DrawColoniesRegion over the current view (DGROUP:0x4c44/0x4c46 origin,
 * 0x4c50/0x4c52 size).  The painter is -called and defined in-file as
 * a stub.
 */

#include "colonization.h"

extern int g_stub_a;

extern void DrawColoniesRegion(int x, int y, int w, int h);

void draw_colonies_view(void)
{
    DrawColoniesRegion(g_view_x0, g_view_y0, g_view_w, g_view_h);
}

