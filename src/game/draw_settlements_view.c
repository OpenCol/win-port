/* draw_settlements_view: candidate for 1040:09a5 (61 bytes).  Verdict: RELOCATABLE.
 * symbol: draw_settlements_view
 *
 * DrawVillagesRegion over the current view (DGROUP:0x4c44/0x4c46 origin,
 * 0x4c50/0x4c52 size).  The painter is -called and defined in-file as
 * a stub.
 */

#include "colonization.h"

extern int g_stub_a;

extern void DrawVillagesRegion(int x, int y, int w, int h);

void draw_settlements_view(void)
{
    DrawVillagesRegion(g_view_x0, g_view_y0, g_view_w, g_view_h);
}

