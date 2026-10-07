/* PORT: this file's own declarations of GamePrefs_GetWindow, win_minimize removed: the header declares them from the definition (W1 step 3). */
/* ColChild_MinButton: candidate for 1048:0c8f (133 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_MinButton
 * name-source: sibling-build
 *
 * On minimizing a game window: its placement record (preferences root SEG20:0x10, slot at
 * +0x772) gets code 1 when the window's flag at +0x148 is set, and the window is minimized
 * through the module's wrapper (`win_minimize_of` -> `win_minimize`) (defined here because the shipped call is the compiler's own
 * `push cs / call `).
 *
 * RENAMED 2026-09-26 from mark_placement: the Mac build's ColChild::MinButton(short): the name audit found this is the action behind the title-bar minimize button (it records code 1, minimized, in the window's placement record and minimizes the window), and LAYOUT had already put MinButton here -- between ColChild_OnMove and ColChild_DrawTitleBar on both sides -- held only for arity; a handler that reads no argument takes none, the Windows handlers' usual shape.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, GamePrefs removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-







#include "records.h"


extern void *GetEventWindow(void);

extern "C" void win_minimize_of(DialogWin *w);

void ColChild_MinButton(void)
{
    ColChild *o;
    PlacementRec *p;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ColChild *)w;
    p = GamePrefs_GetWindow(&g_prefs_root, o->which);
    if (p != 0 && o->save_flag != 0)
        p->state = 1;
    win_minimize_of(&(DialogWin &)*o);
}
