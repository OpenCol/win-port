/* EuropeWin_EuropeError: candidate for 1048:1a8c (81 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeError
 * name-source: sibling-build
 *
 * Replaces whatever the colony child is showing with a one-word nation message: make the
 * message's own nation current, cancel the current message, append the indexed word through
 * `EuropeWin_EuropeMessage` -- which sits 58 bytes above and makes the nation current
 * a second time -- and activate it with kind 3.
 *
 * The trailing argument is a `long` and goes to `EuropeWin_EuropeActivateMessage` untouched.
 *
 * RENAMED 2026-09-26 from show_nation_message: the Mac build's EuropeWin::EuropeError(short, long): its three callees -- set_europe, ColChild::CancelMessage, EuropeWin::EuropeMessage, in that order -- are this function's (plus the Windows redraw), and the arity is (this, word, ms) on both sides. It shows an error word from the Europe message table; 'nation' in the old name came only from the set_europe setup call.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColChild_CancelMessage(EuropeWin *m, int a);
extern void EuropeWin_EuropeActivateMessage(EuropeWin *m, int kind, long ms);

/* `set_europe` is the 5-byte `NOP; PUSH CS; CALL ` form -- TLINK converting a
 * call -- so it stays `extern`; an in-file definition emits the 4-byte form and misses.
 * `EuropeWin_EuropeMessage` is the 4-byte form and is stubbed. */
extern void set_europe(int n);
extern "C" void EuropeWin_EuropeMessage(EuropeWin *m, int word);

void EuropeWin_EuropeError(EuropeWin *win, int word, long ms)
{
    set_europe(win->nation);
    ColChild_CancelMessage(win, 1);
    EuropeWin_EuropeMessage(win, word);
    EuropeWin_EuropeActivateMessage(win, 3, ms);
}
