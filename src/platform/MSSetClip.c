/* MSSetClip: candidate for 1070:af4e (14 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An empty function: prologue, epilogue, nothing between. One of **126** in the image,
 * all byte-identical, and `text_close` carries the reading of what they are and why the
 * Macintosh's zero-argument `memory_check(void)` is NOT adopted for them.
 *
 * The BODY says nothing about the parameters -- it reads none of them. The argument list
 * here is the one `GRPort_SetClip.c` declares for this address, which is where it can be
 * seen.
 *
 * Recorded because that is the useful fact: a call site that looks like it does something
 * does not, and a reader who finds one need not follow it.
 *
 * RENAMED 2026-09-27 from empty_1070_af4e: the Mac build's MSSetClip, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Empty; GRPort_SetClip's one call, empty_1070_af4e(p->win, &p->rect), sits where Mac GRPort::SetClip makes its only call, MSSetClip(CGrafPort*, Rect*). Windows clips through the port rectangle, so the QuickDraw clip-region work (hence ratio 27) is Mac-only. Nothing competes.
 */

#include "colonization.h"

void MSSetClip(void *w, void *r)
{
}
