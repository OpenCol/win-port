/* MSDoTask_1068_d906: candidate for 1068:d906 (23 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSDoTask`, named for the routine it reaches -- the
 * task pump the palette fade calls between steps (`port_fade_through_black`).
 *
 * 23 bytes, not the census's 19: the trailing `return;` is written out, which costs a second
 * epilogue that nothing jumps to (rule 44).
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void MSDoTask_1068_d906(void)
{
    MSDoTask();
    return;
}
