/* EuropeWin_MouseMove: candidate for 1048:a14c (263 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_MouseMove
 *
 * Mouse movement over the Europe window, guarded by a re-entry flag at DGROUP:0x1fda: the
 * window's nation is made current and, inside the dock rectangle (+0x76a..+0x770), the
 * pointer is translated to dock coordinates -- EuropeMouseArea decides between the two handlers.
 *
 * NAME SWAPPED 2026-09-26 with `EuropeWin_LeftMouseClick`. This function is installed on handler slot +0, and the
 * shipped object_message_handler dispatches that slot on WM_MOUSEMOVE. Finding 59 had attached the
 * Mac names by slot from the premise that +4 is the mouse-move slot; the compare chain says +0
 * is. The description below is of this body; only its framing words were changed.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

extern void check_music(void);
extern "C" void set_europe(int n);
extern "C" int EuropeMouseArea(int dx, int dy);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void EuropeWin_EuropeMouseMarket(EuropeWin *o, int dx, int dy);
extern "C" void europe_remove_label(void);

void EuropeWin_MouseMove(int x, int y)
{
    EuropeWin *o;
    EventWin *w;

    check_music();
    if (g_europe_hover_busy != 0)
        return;
    g_europe_hover_busy = 1;
    w = (EventWin *)GetEventWindow();
    o = (EuropeWin *)w;
    set_europe(o->nation);
    if (o->rect.left > x || o->rect.right < x || o->rect.top > y || o->rect.bottom < y)
        goto done;
    g_europe_hover = 1;
    g_pedia_mode = 0;
    g_drag_mode = 0;
    if (EuropeMouseArea(x - o->rect.left, y - o->rect.top) == 0)
        EuropeWin_EuropeMouseMarket(o, x - o->rect.left, y - o->rect.top);
    else
        europe_remove_label();
done:
    MSDoAllTasks_1048_d701();
    g_europe_hover_busy = 0;
}
