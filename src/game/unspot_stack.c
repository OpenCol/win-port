/* unspot_stack: candidate for 1038:272f (57 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: unspot_stack
 *
 * unspot_unit on every unit of the chain whose tail u leads to.
 * The three helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from unit_chain_apply_270a: unspot_unit on every unit of the chain, from its tail.
 *
 * RENAMED 2026-09-26 from unit_chain_clear_flags_high: the Mac build's unspot_stack(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the second of the two functions between unit_activate and spot_unit, which are already bound, paired in order; the arity agrees and the Mac body is 1.23x this one.
 */

#include "colonization.h"

extern int g_stub_a;
extern int top_unit(int u);
extern int next_unit(int u);
extern void unspot_unit(int u);

void unspot_stack(int u)
{
    u = top_unit(u);
    if (u < 0)
        goto done;
    do {
        unspot_unit(u);
        u = next_unit(u);
    } while (u >= 0);
done:
    ;
}
