/* report_error_1040_aa02: candidate for 1040:aa02 (14 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * An empty function: prologue, epilogue, nothing between. One of **126** in the image,
 * all byte-identical, and `text_close` carries the reading of what they are and why the
 * Macintosh's zero-argument `memory_check(void)` is NOT adopted for them.
 *
 * The BODY says nothing about the parameters -- it reads none of them. The argument list
 * here is the one `heap_get.c` declares for this address, which is where it can be
 * seen.
 *
 * Recorded because that is the useful fact: a call site that looks like it does something
 * does not, and a reader who finds one need not follow it.
 *
 * RENAMED 2026-09-26 from empty_1040_aa02: a copy of the error reporter (report_error, 1078:334b) whose body the release build
 * compiled out: every call passes &g_error_log and an "Error: ..." / "Warning: ..." message.
 */

#include "colonization.h"

void report_error_1040_aa02(void *ctx, char *msg, int id, unsigned n)
{
}
