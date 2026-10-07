/* win_minimize: candidate for 1078:2158 (35 bytes).  Verdict: RELOCATABLE.
 *
 * Minimises the window, ShowWindow with SW_MINIMIZE; a null window is ignored.
 */

#include "colonization.h"

#include "records.h"

void win_minimize(MSWin *win) { if (win != 0) ShowWindow(win->hwnd, 6); }
