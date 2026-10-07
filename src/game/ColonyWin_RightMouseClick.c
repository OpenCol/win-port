/* ColonyWin_RightMouseClick: candidate for 1010:4a73 (342 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_RightMouseClick
 *
 * A right click in the colony window (re-entry byte at DGROUP:0x9f7): the window is
 * notified first; outside the client rectangle it is notified again and nothing else
 * happens; inside, the panel and window-relative position are recorded (+0xa0a..+0xa0e,
 * DGROUP:0x9e6) and ColonyWin_ColonyMouseParse handles the panel. MSDoAllTasks_1010_e5bd runs either way.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int ColonyMouseArea(int x, int y);
extern void ColonyWin_ColonyMouseParse(ColonyWin *o, int panel, int dx, int dy);

void ColonyWin_RightMouseClick(int x, int y)
{
    ColonyWin *o;
    EventWin *w;

    if (g_colony_rclick_busy != 0)
        return;
    g_colony_rclick_busy = 1;
    w = (EventWin *)GetEventWindow();
    o = (ColonyWin *)w;
    win_show_and_focus_1010_e4ff(&(DialogWin &)*o);
    g_pedia_mode = 1;
    g_9dd = 0x9b;
    if (o->rect.left > x || o->rect.right < x || o->rect.top > y || o->rect.bottom < y) {
        win_show_and_focus_1010_e4ff(&(DialogWin &)*o);
    } else {
        g_hover_mode = 0;
        o->panel = g_hit_panel = ColonyMouseArea(x - o->rect.left, y - o->rect.top);
        g_colony_drag_armed = 0;
        o->cx = x - o->rect.left;
        o->cy = y - o->rect.top;
        ColonyWin_ColonyMouseParse(o, ColonyMouseArea(x - o->rect.left, y - o->rect.top), x - o->rect.left, y - o->rect.top);
    }
    MSDoAllTasks_1010_e5bd();
    g_colony_rclick_busy = 0;
}
