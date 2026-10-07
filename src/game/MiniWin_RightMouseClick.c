/* MiniWin_RightMouseClick: candidate for 1060:0839 (136 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * If (x, y) lies inside the current window's rectangle (+0x77e..+0x784), subtracts the
 * window's origin from the two parameters -- and returns nothing, so the result is lost.
 * Reproduced as shipped.
 *
 * RENAMED 2026-09-26 from point_to_current_window: the Mac build's MiniWin::RightMouseClick(short, short): MiniWin_Init installs it as the window's rbutton_down handler, and it takes the same two shorts.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

void MiniWin_RightMouseClick(int x, int y)
{
    MiniWin *c;
    char *w = (char *)GetEventWindow();

    c = (MiniWin *)(w != 0 ? w - 0x26 : w);
    if (c->clip.left > x || c->clip.right < x || c->clip.top > y || c->clip.bottom < y)
        return;
    x -= c->clip.left;
    y -= c->clip.top;
}
