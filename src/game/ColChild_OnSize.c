/* PORT: this file's own declarations of GamePrefs_GetWindow, MSIsWindowMaximized, MSIsWindowMinimized removed: the header declares them from the definition (W1 step 3). */
/* ColChild_OnSize: candidate for 1048:0a3b (333 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_OnSize
 * name-source: sibling-build
 *
 * The other end of `ColChild_OnMove`, 0x14d bytes above it: it re-lays the
 * current window's client rectangle at +0x76a -- inset 0xa from the left and 0x20 from the
 * top, ten short of the window's own width and height -- notifies through handler slot 0x1894,
 * and then records the placement with THE SAME twenty-line tail the closer uses: iconic is 1
 * and nothing more; zoomed is 2 and otherwise 0, both with the rectangle copied out field by field.
 *
 * The placement record is looked up FIRST, before any of the layout happens, and only used at
 * the end. A window with no record still gets laid out and still notifies.
 *
 * The `- 0x26` walk back from `GetEventWindow` is the same conditional the closer does, and for
 * the same reason: the null case must not be adjusted.
 *
 * `ColChild_DrawTitleBar` comes through the five-byte NOP form and is extern; every other callee
 * has the four-byte form and is a stub here (rule 47).
 *
 * RENAMED 2026-09-26 from window_resized: the Mac build's ColChild::OnSize(short): the size handler, taking the size code and re-laying the current window's client rectangle. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, GamePrefs removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-







#include "records.h"


extern void *GetEventWindow(void);

extern void MSSetFocus_of_1048_e0cd(Screen *w);
extern void DialogWin_SetDefaultControlFocus(DialogWin *w);
extern void MSGetSizeParent(MSWin *win, RECT *r);

/* 5-byte NOP form, so extern (rule 47) */
extern void ColChild_DrawTitleBar(ColChild *o);

extern "C" int MSIsWindowMinimized_of_1048_d8ad(Screen *w);

extern "C" int MSIsWindowMaximized_of(Screen *w);

extern "C" void MSGetSizeParent_of(Screen *w, RECT *r);

extern "C" int port_width_1048_d853(GRPort *port);
extern "C" int port_height_1048_d86c(GRPort *port);
extern "C" void call_handler_slot_1894(ColChild *w, int size_code);

void ColChild_OnSize(int size_code)
{
    ColChild *o;
    PlacementRec *p;
    RECT rc;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ColChild *)w;
    p = GamePrefs_GetWindow(&g_prefs_root, o->which);
    SetRect(&o->rect, 0xa, 0x20, port_width_1048_d853(o) - 10, port_height_1048_d86c(o) - 10);
    ColChild_DrawTitleBar(o);
    call_handler_slot_1894(o, size_code);
    MSDoAllTasks_1048_d701();
    if (p != 0 && o->save_flag != 0) {
        if (MSIsWindowMinimized_of_1048_d8ad(&(DialogWin &)*o))
            p->state = 1;
        else {
            if (MSIsWindowMaximized_of(&(DialogWin &)*o))
                p->state = 2;
            else
                p->state = 0;
            MSGetSizeParent_of(&(DialogWin &)*o, &rc);
            p->rect.left = rc.left;
            p->rect.top = rc.top;
            p->rect.right = rc.right;
            p->rect.bottom = rc.bottom;
        }
    }
}
