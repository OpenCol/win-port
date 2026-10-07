/* MiniWin_MiniUpdate: candidate for 1060:0721 (75 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A repaint-style handler of module 1060: the current window's owner is passed to
 * MiniWin_Draw (a call TLINK converted).
 *
 * RENAMED 2026-09-26 from owner_call_08dd: MiniWin_Draw on the current window's owner; MiniWin_Init installs
 * it as the map window's repaint handler and calls it once directly.
 *
 * RENAMED 2026-09-26 from redraw_owner_map: the Mac build's MiniWin::MiniUpdate(void), bound by LAYOUT: on both sides it is the one function between MiniWin_Init and MiniWin_LeftMouseClick. The Windows handler takes no this -- it asks GetEventWindow() for the window and hands it to MiniWin_Draw -- which is the Windows handlers' usual shape; the Mac body is 0.83x this one. RE_STATUS had left MiniUpdate open for want of a caller or slot that named it.
 */

#include "colonization.h"

#include "w16types.h"
extern void *GetEventWindow(void);
#include "classes.h"
extern void MiniWin_Draw(MiniWin *o);

void MiniWin_MiniUpdate(void)
{
    char *o;
    char *w = (char *)GetEventWindow();

    o = w != 0 ? w - 0x26 : w;
    MiniWin_Draw((MiniWin *)o);
}
