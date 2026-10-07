/* ColonyWin_LeftMouseClick: candidate for 1010:4911 (354 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_LeftMouseClick
 *
 * A left button press in the colony window (re-entry byte at DGROUP:0x9f6): the window is
 * notified; outside the client rectangle it is notified again and the function returns
 * without clearing the re-entry byte (as shipped); inside, the panel and position are
 * recorded, the drag state set (`g_colony_clicked` and `g_colony_drag_armed` to 1,
 * `g_hover_mode` to 0), and ColonyWin_ColonyMouseParse
 * handles the panel.
 *
 * NAME SWAPPED 2026-09-26 with `ColonyWin_MouseMove`. This function is installed on handler slot +4, and the
 * shipped object_message_handler dispatches that slot on WM_LBUTTONDOWN. Finding 59 had attached the
 * Mac names by slot from the premise that +4 is the mouse-move slot; the compare chain says +0
 * is. The description below is of this body; only its framing words were changed.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int ColonyMouseArea(int x, int y);
extern void ColonyWin_ColonyMouseParse(ColonyWin *o, int panel, int dx, int dy);

void ColonyWin_LeftMouseClick(int x, int y)
{
    ColonyWin *o;
    EventWin *w;

    if (g_colony_lclick_busy != 0)
        return;
    g_colony_lclick_busy = 1;
    w = (EventWin *)GetEventWindow();
    o = (ColonyWin *)w;
    win_show_and_focus_1010_e4ff(&(DialogWin &)*o);
    g_pedia_mode = 0;
    g_9dd = 0;
    if (o->rect.left > x || o->rect.right < x || o->rect.top > y || o->rect.bottom < y) {
        win_show_and_focus_1010_e4ff(&(DialogWin &)*o);
        return;
    }
    o->panel = g_hit_panel = ColonyMouseArea(x - o->rect.left, y - o->rect.top);
    g_colony_drag_armed = 1;
    g_colony_clicked = 1;
    g_hover_mode = 0;
    o->cx = x - o->rect.left;
    o->cy = y - o->rect.top;
    ColonyWin_ColonyMouseParse(o, ColonyMouseArea(x - o->rect.left, y - o->rect.top), x - o->rect.left, y - o->rect.top);
    MSDoAllTasks_1010_e5bd();
    g_colony_lclick_busy = 0;
}
