/* text_close: candidate for 1040:6ce6 (14 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An empty function, and unlike the other two in this corpus it is not a handler slot:
 * it has **14 call sites** across six segments, most of them the last call a function
 * makes before its epilogue. Something was compiled out of this build.
 *
 * It sits immediately after `heap_get`, in the middle of the heap and resource
 * code, and the Macintosh build has a zero-argument `memory_check(void)` -- which would fit
 * a debug heap validator called at scattered checkpoints and left empty in a release build.
 * That name is NOT adopted. An empty body offers no behavioural evidence at all, the
 * Macintosh table has about 130 zero-argument functions, and the call sites do not agree:
 * two follow `set_name_slot`, one follows `sound_play`, one a counter increment, and
 * two are guarded by unrelated comparisons. Adjacency and arity are the whole case, and
 * they are not enough.
 *
 * Recorded as empty because that is the useful fact: 14 call sites that look like they do
 * something do not, and a reader who finds one need not follow it.
 *
 * RENAMED 2026-09-26 from empty_1040_6ce6: the Mac build's text_close(void), bound by call-sequence alignment: in text_search the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (0), and the body does what the Mac name says. The Windows text_close is empty.
 */

#include "colonization.h"

void text_close(void)
{
}
