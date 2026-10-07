/* MSShowMenuBar: candidate for 1070:dad8 (14 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An empty function: prologue, epilogue, nothing between. One of **126** in the image,
 * all byte-identical, and `text_close` carries the reading of what they are and why the
 * Macintosh's zero-argument `memory_check(void)` is NOT adopted for them.
 *
 * Recorded because that is the useful fact: a call site that looks like it does something
 * does not, and a reader who finds one need not follow it.
 *
 * RENAMED 2026-09-27 from empty_1070_dad8: the Mac build's MSShowMenuBar, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Empty, next to empty_1070_daca in the menu block; its callers (EuropeWin_Close, EuropeWin_OnOKClick, ColonyWin_OnOKClick, CloseBackdrop, AboutBox...) are the Mac MSShowMenuBar callers; Mac-only work, nothing competes.
 */

#include "colonization.h"

void MSShowMenuBar(void)
{
}
