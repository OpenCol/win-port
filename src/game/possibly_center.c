/* possibly_center: candidate for 1038:b07d (291 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: possibly_center
 *
 * Scrolls the map when the rectangle (x1, y1)-(x2, y2) comes within four
 * tiles of the view's edge (DGROUP:0x4c44..0x4c4a, the map size at
 * 0x4c40/0x4c42 bounding the edges): set_center to (x1, y1) with `flag`
 * and the window recentred.  Returns whether it scrolled.  Four helpers
 * are -called and defined in-file (min/max real, the movers stubs).
 *
 * RENAMED 2026-09-26 from map_scroll_to_show: the Mac build's possibly_center(short, short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller MoveUnit; caller SlideIcon; caller indian_move; caller perform_orders_build; caller perform_orders_clear), the runner-up gets 2, and the arity agrees (5).
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
#include "classes.h"

extern int g_stub_a;

extern void set_center(int x, int y, int flag);

int possibly_center(int x1, int y1, int x2, int y2, int flag)
{
    register int r = 0;
    int xmin;
    int ymin;
    int xmax;
    int ymax;

    xmin = min_int_1038_e446(x1, x2);
    xmax = max_int_1038_e46c(x1, x2);
    ymin = min_int_1038_e446(y1, y2);
    ymax = max_int_1038_e46c(y1, y2);
    if (g_view_x0 + 4 > xmin && g_view_x0 > 1)
        r = 1;
    if (g_view_y0 + 4 > ymin && g_view_y0 > 1)
        r = 1;
    if (g_view_x1 - 4 < xmax && g_view_x1 < g_map_size.w - 4)
        r = 1;
    if (g_view_y1 - 4 < ymax && g_view_y1 < g_map_size.h - 4)
        r = 1;
    if (r != 0) {
        set_center(x1, y1, flag);
        MapWin_Recenter(&g_map_window, x1, y1);
    }
    return r;
}
