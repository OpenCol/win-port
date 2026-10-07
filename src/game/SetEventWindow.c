/* SetEventWindow: candidate for 1060:9fe6 (49 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Replaces the current-window pointer at DGROUP:0x2ae0 and returns the previous one.
 *
 * RENAMED 2026-09-27 from set_current_window: the Mac build's SetEventWindow(EventWin*), GetEventWindow's partner: it swaps g_current_window and returns the old one; the control wndprocs call it before they dispatch.
 */

#include "colonization.h"

void *SetEventWindow(void *w)
{
    void *old = g_current_window;
    g_current_window = w;
    return old;
}
