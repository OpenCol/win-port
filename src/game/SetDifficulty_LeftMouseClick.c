/* SetDifficulty_LeftMouseClick: candidate for 1030:091b (96 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SetDifficulty_LeftMouseClick
 * name-source: sibling-build
 *
 * Takes the current window, steps back to the object that owns it, hit-tests the window's regions
 * at the click point and hands the answer to this module's handler. One of three
 * per-module copies differing only in which pair of functions they call.
 *
 * RENAMED 2026-09-27 from dispatch_click_1030_091b: LEADS3 (verified by another agent, accepted; checked for conflicts with this batch: none). Found while checking lead 1. Mac CODE 17 at 0x4608..0x4640 is closed by the long-form name 'LeftMouseClick__13SetDifficultyFss', which the extractor missed. Layout: second in the SetDifficulty block on both sides, after OkButtonClick. Arity: (short, short) against the reconstruction's (int x, int y). Body and size: 0x60 bytes calling GetEventWindow, SetDifficulty_TrackMouse and SetDifficulty_DoMessage. That is the same size and shape as SetNations_LeftMouseClick (1030:0057, 0x60), which calls SetNations_TrackMouse/DoMessage. No collision. SetDifficulty_Init.c's header prose (line 21, 'stay unnamed') needs correcting when this is applied.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);
extern int SetDifficulty_TrackMouse(SetDifficulty *w, int x, int y);
extern void SetDifficulty_DoMessage(SetDifficulty *w, int panel);
void SetDifficulty_LeftMouseClick(int x, int y)
{
    SetDifficulty *c;
    EventWin *w = (EventWin *)GetEventWindow();

    c = (SetDifficulty *)w;
    SetDifficulty_DoMessage(c, SetDifficulty_TrackMouse(c, x, y));
}
