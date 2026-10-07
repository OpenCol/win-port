/* ATOL_1028_e4e9: candidate for 1028:e4e9 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 32, not the census 28. The trailing 4 bytes are the SECOND epilogue that
 * the `return` adds (rule 44), which Ghidra excludes because nothing reaches it.
 *
 * This module's one-line forwarder to `ATOL`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

long ATOL_1028_e4e9(void *p)
{
    return ATOL(p);
}
