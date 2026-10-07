/* EuropeWin_LeftMouseClick: candidate for 1048:a025 (295 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_LeftMouseClick
 *
 * A left button press in the Europe window (re-entry flag at DGROUP:0x1fd8): outside the dock
 * rectangle the window is handled by win_show_and_focus_1048_d8f7; inside, the dock slot is looked up twice
 * -- once with the result discarded (EuropeMouseArea is a pure hit test, so that call has no effect), once for EuropeWin_EuropeMouseParse.
 *
 * NAME SWAPPED 2026-09-26 with `EuropeWin_MouseMove`. This function is installed on handler slot +4, and the
 * shipped object_message_handler dispatches that slot on WM_LBUTTONDOWN. Finding 59 had attached the
 * Mac names by slot from the premise that +4 is the mouse-move slot; the compare chain says +0
 * is. The description below is of this body; only its framing words were changed.
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

void EuropeWin_LeftMouseClick(int x, int y)
{
    EuropeWin *o;
    EventWin *w;

    if (g_europe_lclick_busy != 0)
        return;
    g_europe_lclick_busy = 1;
    w = (EventWin *)GetEventWindow();
    o = (EuropeWin *)w;
    set_europe(o->nation);
    if (o->rect.left > x || o->rect.right < x || o->rect.top > y || o->rect.bottom < y) {
        win_show_and_focus_1048_d8f7(&(DialogWin &)*o);
    } else {
        g_drag_mode = 0;
        g_europe_hover = 0;
        g_pedia_mode = 0;
        EuropeMouseArea(x - o->rect.left, y - o->rect.top);
        EuropeWin_EuropeMouseParse(o, EuropeMouseArea(x - o->rect.left, y - o->rect.top), x - o->rect.left, y - o->rect.top);
    }
    MSDoAllTasks_1048_d701();
    g_europe_lclick_busy = 0;
}
