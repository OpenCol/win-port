/* bottom_box: candidate for 1038:af8c (14 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An empty function: prologue, epilogue, nothing between. One of **126** in the image,
 * all byte-identical, and `text_close` carries the reading of what they are and why the
 * Macintosh's zero-argument `memory_check(void)` is NOT adopted for them.
 *
 * Recorded because that is the useful fact: a call site that looks like it does something
 * does not, and a reader who finds one need not follow it.
 *
 * RENAMED 2026-09-26 from empty_1038_af8c: the Mac build's bottom_box(short), bound by call-graph agreement (3 of its Mac call edges already bound land here, runner-up 1) and by call-sequence alignment (1.5 aligned gaps in indian_turn, new_turn_bookkeeping leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.86x this one.
 */

#include "colonization.h"

void bottom_box(int unused1)
{
}
