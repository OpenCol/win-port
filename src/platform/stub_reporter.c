/* stub_reporter: candidate for 1078:334b (14 bytes).  Verdict: EXACT.
 * symbol: report_error
 *
 * The 14-byte __loadds stub that appears 126 times in COLONIZE.EXE -- here, the error
 * reporter the CVPC decompressor's caller invokes with
 * ("Error: Cannot allocate decompres...").
 *
 * The body is empty in the shipped build: 11 instructions of prologue and epilogue and
 * nothing else. Consistent with a diagnostic compiled out of the release build.
 *
 * The argument list cannot be read from this end -- a `RETF` with no immediate is , so
 * the CALLEE says nothing about what it was given, and an empty body compiles to the
 * same 14 bytes for any signature. It CAN be read from the call sites, and all five agree:
 * eight bytes, pushed as two segment-then-offset pairs, so two pointers.
 *
 * They agree on the arguments too. Every site passes `&g_error_log` at DGROUP:0x2ac0 --
 * the same error context `GRPort_CopyToPort` and `CopyToScreen` hand to `report_error_1068_963d` -- and a
 * message string. So this is that mechanism's reporting stub, with the reporting removed.
 *
 * This file previously declared THREE parameters on the strength of one call site read by
 * hand. `tools/signatures.py`, which exists for exactly this, reads five and makes it two.
 * The count was the one thing the header admitted it could not check, and it was wrong.
 */

#include "colonization.h"

void report_error(void *ctx, char *message)
{
}
