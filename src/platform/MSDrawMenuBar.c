/* MSDrawMenuBar: candidate for 1070:d63e (26 bytes).  Verdict: RELOCATABLE.
 *
 * DrawMenuBar(w->hwnd) -- repaints the bar after the menu behind it changed.
 */

#include "colonization.h"

#include "records.h"

void MSDrawMenuBar(MSWin *win)
{
    DrawMenuBar(win->hwnd);
}
