/* SetNations_LeftMouseClick: candidate for 1030:0057 (96 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SetNations_LeftMouseClick
 * name-source: sibling-build
 *
 * Takes the current window, steps back to the object that owns it, hit-tests the window's panels
 * at the click point and hands the answer to this module's handler. One of three
 * per-module copies differing only in which pair of functions they call.
 *
 * RENAMED 2026-09-26 from dispatch_click_1030_0057: the Mac build's SetNations::LeftMouseClick(short, short): SetNations_Init installs it as the dialog's lbutton_down handler, and it takes the same two shorts.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);
extern int SetNations_TrackMouse(SetNations *w, int x, int y);
extern void SetNations_DoMessage(SetNations *w, int panel);
void SetNations_LeftMouseClick(int x, int y)
{
    SetNations *c;
    EventWin *w = (EventWin *)GetEventWindow();

    c = (SetNations *)w;
    SetNations_DoMessage(c, SetNations_TrackMouse(c, x, y));
}
