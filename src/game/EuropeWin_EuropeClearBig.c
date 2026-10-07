/* EuropeWin_EuropeClearBig: candidate for 1048:1bdf (54 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeClearBig
 * name-source: sibling-build
 *
 * Makes the window's nation current and copies a fixed band -- (0, -2) to (640, 248) --
 * out of the window's saved port at +0x884 and back into the window, offset to wherever
 * the window sits. The repaint half of the pair whose other half,
 * `EuropeWin_EuropeShowBig` (1048:1c15) at the very next address, pushes a band the
 * other way, from the window to the screen.
 *
 * The two bands are not the same rectangle: this one starts two pixels ABOVE the window
 * origin and is 250 tall, the screen one starts eight below. Both are literals, so
 * neither is read from the window; only the origin at +0x76a moves them.
 *
 * `set_europe` and `EuropeWin_UpdateEurope` are both reached by the five-byte
 * NOP form, so both are `extern` (rule 47).
 *
 * RENAMED 2026-09-26 from restore_window_band_from_port: the Mac build's EuropeWin::EuropeClearBig(void), bound by call-graph agreement: 2 of its Mac call edges already bound land on this function (callee UpdateEurope; callee set_europe), the runner-up gets 1; the arity agrees (1), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void set_europe(int n);
extern void EuropeWin_UpdateEurope(EuropeWin *w, int x, int y, int w2, int h);

void EuropeWin_EuropeClearBig(EuropeWin *w)
{
    set_europe(w->nation);
    EuropeWin_UpdateEurope(w, 0, -2, 0x280, 0xfa);
}
