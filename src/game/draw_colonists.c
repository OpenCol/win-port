/* draw_colonists: candidate for 1010:be4c (14 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An empty function: prologue, epilogue, nothing between. One of **126** in the image,
 * all byte-identical, and `text_close` carries the reading of what they are and why the
 * Macintosh's zero-argument `memory_check(void)` is NOT adopted for them.
 *
 * The BODY says nothing about the parameters -- it reads none of them. The argument list
 * here is the one `colony_cursor_update.c` declares for this address, which is where it can be
 * seen.
 *
 * Recorded because that is the useful fact: a call site that looks like it does something
 * does not, and a reader who finds one need not follow it.
 *
 * RENAMED 2026-09-27 from empty_1010_be4c: the Mac build's draw_colonists, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Empty. It is the 1st of colony_cursor_update's five calls, where Mac colony_cursor_update calls draw_colonists first; the five Windows addresses (bdea<be4c<bee3<bef1<bf0d) are in the same order as the Mac seg-31 layout (map, colonists, ships, misc, market); runner-up 0.0.
 */

#include "colonization.h"

void draw_colonists(int unused1)
{
}
