/* move_cursor: candidate for 1038:b1cb (14 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An empty function: prologue, epilogue, nothing between. One of **126** in the image,
 * all byte-identical, and `text_close` carries the reading of what they are and why the
 * Macintosh's zero-argument `memory_check(void)` is NOT adopted for them.
 *
 * The BODY says nothing about the parameters -- it reads none of them. The argument list
 * here is the one `GameBrains_MoveUnit.c` declares for this address, which is where it can be
 * seen.
 *
 * Recorded because that is the useful fact: a call site that looks like it does something
 * does not, and a reader who finds one need not follow it.
 *
 * RENAMED 2026-09-26 from empty_1038_b1cb: the Mac build's move_cursor(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between force_map and viceroy_activate_message, which are already bound; the arity agrees and the Mac body is 1.86x this one.
 */

#include "colonization.h"

void move_cursor(int dir)
{
}
