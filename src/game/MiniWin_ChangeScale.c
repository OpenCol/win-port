/* MiniWin_ChangeScale: candidate for 1060:08cf (14 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An empty function: prologue, epilogue, nothing between. One of **126** in the image,
 * all byte-identical, and `text_close` carries the reading of what they are and why the
 * Macintosh's zero-argument `memory_check(void)` is NOT adopted for them.
 *
 * The BODY says nothing about the parameters -- it reads none of them. The argument list
 * here is the one `MainWin_ResetWindows.c` declares for this address, which is where it can be
 * seen.
 *
 * Recorded because that is the useful fact: a call site that looks like it does something
 * does not, and a reader who finds one need not follow it.
 *
 * RENAMED 2026-09-26 from empty_1060_08cf: the Mac build's MiniWin::ChangeScale(short): it sits inside the MiniWin module (1060:05e8..15d0), and its one caller, MainWin_ResetWindows, calls it on the minimap (o->mini) with a scale of 3. The Windows body is empty -- this port keeps the overview at a fixed scale -- but the name comes from that call, not from the empty-function family, and no other empty copy takes it. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
void MiniWin_ChangeScale(MiniWin *w, int scale)
{
}
