/* EventWin_EndModal: candidate for 1060:9fad (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Ends a modal session: clears the modal-window pointer `g_modal_win` (DGROUP:0x2ae4, a 
 * pointer stored as one 32-bit zero) and the word at +0x76 of the port it is given. Its callers
 * pass a window's port -- `ReportWin_Done` hands it `w->port`, and a button handler in
 * module 1050 hands it the current dialog owner's port.
 *
 * RENAMED 2026-09-26 from end_modal: the Mac build's EventWin::EndModal(void): this only, clearing the modal window and the +0x76 flag EventWin_Modal set. The width matches.
 */

#include "colonization.h"

#include "classes.h"

void EventWin_EndModal(EventWin *win)
{
    g_modal_win = 0;
    win->running = 0;
}
