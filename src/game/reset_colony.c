/* reset_colony: candidate for 1010:41b0 (14 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An empty function: prologue, epilogue, nothing between. One of **126** in the image,
 * all byte-identical, and `text_close` carries the reading of what they are and why the
 * Macintosh's zero-argument `memory_check(void)` is NOT adopted for them.
 *
 * Recorded because that is the useful fact: a call site that looks like it does something
 * does not, and a reader who finds one need not follow it.
 *
 * RENAMED 2026-09-26 from empty_1010_41b0: the Mac build's reset_colony(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between colony_custom_house and colony_change_mode, which are already bound; the arity agrees and the Mac body is 2.00x this one.
 */

#include "colonization.h"

void reset_colony(void)
{
}
