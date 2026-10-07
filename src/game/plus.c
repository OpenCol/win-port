/* plus: candidate for 1030:1cad (70 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: plus
 *
 * Adds a delta to an unsigned byte and stores it back clamped to 255; the clamp is min_ptr_1030_f474 over
 * pointers to two temporaries (rule 26). Each is stored just before its address is pushed, right to
 * left, which is what `&(b = 0xff)` inside the call gives; separate assignment statements store both first.
 *
 * RENAMED 2026-09-26 from add_clamped_255: the Mac build's plus(unsigned char*, short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller collect_indian_stats; caller collect_stats), the runner-up gets 1, the arity agrees (2), and the Mac body is 0.86x this one's size. The margin is small, so the binding also rests on the body doing what the Mac name says.
 */

#include "colonization.h"

extern "C" void plus(unsigned char *p, int delta)
{
    int b, a;

    *p = (unsigned char)*min_ptr_1030_f474(&(a = *p + delta), &(b = 0xff));
}
