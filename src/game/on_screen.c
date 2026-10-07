/* on_screen: candidate for 1038:b5c7 (57 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * 1 when (x, y) lies within the view rectangle at DGROUP:0x4c44..0x4c4a (left, top,
 * right, bottom inclusive).
 *
 * RENAMED 2026-09-26 from in_map_rect: the Mac build's on_screen(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller CursorUpdate; caller DoHuman), the runner-up gets 1, and the arity agrees (2). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

int on_screen(int x, int y)
{
    int r = 1;

    if (x < g_view_x0 || x > g_view_x1)
        r = 0;
    if (y < g_view_y0 || y > g_view_y1)
        r = 0;
    return r;
}
