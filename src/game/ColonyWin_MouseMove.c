/* ColonyWin_MouseMove: candidate for 1010:47e1 (304 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_MouseMove
 *
 * Mouse movement over the colony window: outside the client rectangle (+0x76a..+0x770) the
 * window goes to win_show_and_focus_1010_e4ff; inside, the panel under the pointer (ColonyMouseArea) and its
 * window-relative position are stored at +0xa0a/+0xa0c/+0xa0e, the drag state words
 * at DGROUP:0x9e4/0x9e8 cleared and 0x9ea set, and ColonyWin_ColonyMouseMove handles the panel.
 *
 * NAME SWAPPED 2026-09-26 with `ColonyWin_LeftMouseClick`. This function is installed on handler slot +0, and the
 * shipped object_message_handler dispatches that slot on WM_MOUSEMOVE. Finding 59 had attached the
 * Mac names by slot from the premise that +4 is the mouse-move slot; the compare chain says +0
 * is. The description below is of this body; only its framing words were changed.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

extern void check_music(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int ColonyMouseArea(int x, int y);
extern void ColonyWin_ColonyMouseMove(ColonyWin *o, int panel, int dx, int dy);

void ColonyWin_MouseMove(int x, int y)
{
    ColonyWin *o;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ColonyWin *)w;
    check_music();
    g_pedia_mode = 0;
    if (o->rect.left > x || o->rect.right < x || o->rect.top > y || o->rect.bottom < y) {
        win_show_and_focus_1010_e4ff(&(DialogWin &)*o);
        return;
    }
    o->panel = ColonyMouseArea(x - o->rect.left, y - o->rect.top);
    g_colony_drag_armed = 0;
    g_colony_clicked = 0;
    o->cx = x - o->rect.left;
    o->cy = y - o->rect.top;
    g_hover_mode = 1;
    ColonyWin_ColonyMouseMove(o, ColonyMouseArea(x - o->rect.left, y - o->rect.top), x - o->rect.left, y - o->rect.top);
}
