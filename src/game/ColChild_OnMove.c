/* ColChild_OnMove: candidate for 1048:0b88 (263 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_OnMove
 * name-source: sibling-build
 *
 * When a game window moves: the window is given the focus and its placement record (preferences
 * root SEG20:0x10, slot at +0x772) updated when the window's flag at +0x148 is set -- code 1
 * when minimised, otherwise 2 when maximised or 0 with the window rectangle recorded. The
 * module's window wrappers are defined here because the shipped calls to them are the
 * compiler's own `push cs / call ` (win_show_and_focus_1048_d8f7's own callee is not in this file).
 *
 * RENAMED 2026-09-26 from save_window_placement: the Mac build's ColChild::OnMove(short, short): game_window_init installs it on the EventTable's move slot, beside ColChild_OnSize on the size slot, and it records the window's placement in the preferences. The body reads no argument, so the Windows prototype has none.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, GamePrefs removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-







#include "records.h"


extern void *GetEventWindow(void);
extern PlacementRec *GamePrefs_GetWindow(void *root, int which);
extern void MSSetFocus_of_1048_e0cd(Screen *w);
extern void DialogWin_SetDefaultControlFocus(DialogWin *w);
extern int MSIsWindowMinimized(void *win);
extern int MSIsWindowMaximized(void *win);
extern void MSGetSizeParent(MSWin *win, RECT *r);

extern "C" void win_show_and_focus_1048_d8f7(DialogWin *w);

extern "C" int MSIsWindowMinimized_of_1048_d8ad(Screen *w);

extern "C" int MSIsWindowMaximized_of(Screen *w);

extern "C" void MSGetSizeParent_of(Screen *w, RECT *r);

void ColChild_OnMove(void)
{
    ColChild *o;
    PlacementRec *p;
    RECT rc;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ColChild *)w;
    win_show_and_focus_1048_d8f7(&(DialogWin &)*o);
    p = GamePrefs_GetWindow(&g_prefs_root, o->which);
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
