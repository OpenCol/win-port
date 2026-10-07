/* unit_activate: candidate for 1038:2676 (148 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: unit_activate
 *
 * Puts unit u on the map at its own x, y (relocate_unit), describes it
 * (explore_square), and, when the tile redraw (possibly_center) reports
 * nothing drawn, refreshes the 7x7 block around it.  The unit becomes the
 * one at SEG20:0x7794.  relocate_unit is -called and defined
 * in-file as a stub.
 *
 * RENAMED 2026-09-26 from unit_arrives: the Mac build's unit_activate(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller move_unit; caller orders_activate; caller orders_disband; callee explore_square), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern int g_stub_a;

extern void DrawMapRegion(int x, int y, int w, int h, int flag);
extern int possibly_center(int x1, int y1, int x2, int y2, int flag);

extern void relocate_unit(int u, int x, int y);

void unit_activate(int u)
{
    int x;
    int y;

    if (u >= 0) {
        x = g_units[u].x;
        y = g_units[u].y;
        relocate_unit(u, x, y);
        explore_square(u);
        if (possibly_center(x, y, x, y, 0) == 0)
            DrawMapRegion(x - 3, y - 3, 7, 7, 1);
    }
    g_game.selected_unit = u;
}
