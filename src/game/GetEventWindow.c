/* GetEventWindow: candidate for 1060:9fcd (25 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The pointer at DGROUP:0x2ae0. SetEventWindow right after it is the writer.
 *
 * RENAMED 2026-09-27 from current_window: the Mac build's GetEventWindow, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Returns g_current_window, which the control wndprocs set through SetEventWindow before they dispatch (Mac SetEventWindow); 0 args, ratio 1.44, and the Mac callers (the Close/DragIcon/DrawAll handlers) are the ones that read it.
 */

#include "colonization.h"

void *GetEventWindow(void)
{
    return g_current_window;
}
