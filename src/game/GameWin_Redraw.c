/* GameWin_Redraw: candidate for 1060:a249 (56 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: GameWin_Redraw
 *
 * Makes the window's DialogWin (+0x26) the current event window (DGROUP:0x2ae0, what GetEventWindow returns)
 * and runs handler slot 192 and copy_all_to_screen_1060_ae72 on it.  Both are -called
 * and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from window_relayout: the Mac build's GameWin::Redraw(void), bound by call-graph agreement (tools/mac_callgraph.py): 10 of its Mac call edges already bound land on this function (caller DoHuman; caller DoViceroy; caller LeftMouseUp; caller MainMenuProc; caller ResetWindows), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "classes.h"

extern int g_stub_a;

extern void call_relayout_handler(void *table);

void GameWin_Redraw(GameWin *w)
{
    g_current_window = (char *)w + 0x26;
    call_relayout_handler(w);
    copy_all_to_screen_1060_ae72(w);
}
