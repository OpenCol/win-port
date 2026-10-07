/* ColonyWin_DblMouseClick: candidate for 1010:4bc9 (260 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DblMouseClick
 * name-source: sibling-build
 *
 * The colony window's slot-28 handler (re-entry byte at DGROUP:0x9f8): records the
 * window-relative position and panel of (x, y) exactly as the click handlers do, with 0x9dd
 * set and `g_colony_drag_armed`/`g_hover_mode` cleared, and hands the panel to
 * ColonyWin_ColonyMouseParse.
 *
 * RENAMED 2026-09-26 from ColonyWin_DoubleClick: the Mac build's ColonyWin::DblMouseClick(short, short): ColonyWin_Init installs it on the double-click slot, and it takes the same two shorts. The old name had the class prefix but not the Mac method.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int ColonyMouseArea(int x, int y);
extern void ColonyWin_ColonyMouseParse(ColonyWin *o, int panel, int dx, int dy);

void ColonyWin_DblMouseClick(int x, int y)
{
    ColonyWin *o;
    EventWin *w;

    if (g_colony_slot28_busy != 0)
        return;
    g_colony_slot28_busy = 1;
    w = (EventWin *)GetEventWindow();
    o = (ColonyWin *)w;
    g_9dd = 1;
    g_hover_mode = 0;
    g_colony_drag_armed = 0;
    o->cx = x - o->rect.left;
    o->cy = y - o->rect.top;
    o->panel = g_hit_panel = ColonyMouseArea(x - o->rect.left, y - o->rect.top);
    ColonyWin_ColonyMouseParse(o, ColonyMouseArea(x - o->rect.left, y - o->rect.top), x - o->rect.left, y - o->rect.top);
    MSDoAllTasks_1010_e5bd();
    g_colony_slot28_busy = 0;
}
