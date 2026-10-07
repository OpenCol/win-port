/* unit_chain_count: candidate for 1038:1b05 (58 bytes).  Verdict: RELOCATABLE.
 * symbol: unit_chain_count
 *
 * How many units follow from the tail of u's chain.  Both helpers are
 * -called and defined in-file as stubs.
 */

#include "colonization.h"

extern int g_stub_a;
extern int top_unit(int u);
extern int next_unit(int u);

int unit_chain_count(int u)
{
    int n = 0;

    for (u = top_unit(u); u >= 0; u = next_unit(u))
        n++;
    return n;
}
