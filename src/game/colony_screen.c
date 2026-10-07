/* colony_screen: candidate for 1010:e265 (14 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An empty function: prologue, epilogue, nothing between. One of **126** in the image,
 * all byte-identical, and `text_close` carries the reading of what they are and why the
 * Macintosh's zero-argument `memory_check(void)` is NOT adopted for them.
 *
 * Recorded because that is the useful fact: a call site that looks like it does something
 * does not, and a reader who finds one need not follow it.
 *
 * RENAMED 2026-09-26 from empty_1010_e265: the Mac build's colony_screen(short), bound by call-sequence alignment (2.0 aligned gaps in combat_fight, move_unit leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 5.14x this one. The Windows body is empty.
 */

#include "colonization.h"

void colony_screen(int unused1)
{
}
