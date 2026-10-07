/* MSHideMenuBar: candidate for 1070:daca (14 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An empty function: prologue, epilogue, nothing between. One of **126** in the image,
 * all byte-identical, and `text_close` carries the reading of what they are and why the
 * Macintosh's zero-argument `memory_check(void)` is NOT adopted for them.
 *
 * Recorded because that is the useful fact: a call site that looks like it does something
 * does not, and a reader who finds one need not follow it.
 *
 * RENAMED 2026-09-27 from empty_1070_daca: the Mac build's MSHideMenuBar, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Empty, sitting straight after MSModifyMenu in the platform menu block; its Windows callers (SuperWin_Init, OpenBackdrop, AboutWin_ctor, run_game...) are the Mac MSHideMenuBar callers; hiding the menu bar is Mac-only, and nothing else competes (runner-up 0.0).
 */

#include "colonization.h"

void MSHideMenuBar(void)
{
}
