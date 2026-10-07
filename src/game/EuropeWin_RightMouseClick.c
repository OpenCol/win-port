/* EuropeWin_RightMouseClick: candidate for 1048:9f1b (266 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_RightMouseClick
 *
 * A right click in the Europe window, guarded by a re-entry flag at DGROUP:0x1fda + -4: the
 * window's nation is made current; outside the dock rectangle the window itself is handled
 * by win_show_and_focus_1048_d8f7, inside it the dock slot under the click goes to EuropeWin_EuropeMouseParse.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

extern "C" void set_europe(int n);
extern "C" int EuropeMouseArea(int dx, int dy);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void EuropeWin_EuropeMouseParse(EuropeWin *o, int slot, int dx, int dy);
extern "C" void win_show_and_focus_1048_d8f7(void *w);

void EuropeWin_RightMouseClick(int x, int y)
{
    EuropeWin *o;
    EventWin *w;

    if (g_europe_rclick_busy != 0)
        return;
    g_europe_rclick_busy = 1;
    w = (EventWin *)GetEventWindow();
    o = (EuropeWin *)w;
    g_drag_mode = 0;
    g_europe_hover = 0;
    g_pedia_mode = 1;
    set_europe(o->nation);
    if (o->rect.left > x || o->rect.right < x || o->rect.top > y || o->rect.bottom < y)
        win_show_and_focus_1048_d8f7(&(DialogWin &)*o);
    else
        EuropeWin_EuropeMouseParse(o, EuropeMouseArea(x - o->rect.left, y - o->rect.top), x - o->rect.left, y - o->rect.top);
    MSDoAllTasks_1048_d701();
    g_europe_rclick_busy = 0;
}
