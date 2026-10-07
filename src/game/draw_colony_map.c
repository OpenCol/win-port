/* draw_colony_map: candidate for 1010:bdea (14 bytes).  Verdict: EXACT.
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
 * RENAMED 2026-09-27 from empty_1010_bdea: the Mac build's draw_colony_map, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Empty; the 2nd call in colony_cursor_update, and the Mac's 2nd is draw_colony_map; its address order agrees with the Mac layout; nothing competes.
 */

#include "colonization.h"

void draw_colony_map(int unused1)
{
}
