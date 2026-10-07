/* win_set_capture: candidate for 1078:2d24 (26 bytes).  Verdict: RELOCATABLE.
 *
 * Captures the mouse to the window's handle.
 */

#include "colonization.h"

#include "records.h"

void win_set_capture(MSWin *win) { SetCapture(win->hwnd); }
