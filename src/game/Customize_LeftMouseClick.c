/* Customize_LeftMouseClick: candidate for 1030:1241 (96 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Customize_LeftMouseClick
 * name-source: sibling-build
 *
 * Takes the current window, steps back to the object that owns it, hit-tests the window's twelve rectangles
 * at the click point and hands the answer to this module's handler. One of three
 * per-module copies differing only in which pair of functions they call.
 *
 * RENAMED 2026-09-26 from dispatch_click_1030_1241: the Mac build's Customize::LeftMouseClick(short, short): Customize_Init installs it as the dialog's lbutton_down handler, and it takes the same two shorts.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);
extern int Customize_TrackMouse(Customize *w, int x, int y);
extern void Customize_DoMessage(Customize *w, int panel);
void Customize_LeftMouseClick(int x, int y)
{
    Customize *c;
    EventWin *w = (EventWin *)GetEventWindow();

    c = (Customize *)w;
    Customize_DoMessage(c, Customize_TrackMouse(c, x, y));
}
