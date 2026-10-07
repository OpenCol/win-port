/* nth_in_chain: candidate for 1038:1a02 (87 bytes).  Verdict: RELOCATABLE.
 * symbol: nth_in_chain
 *
 * The n-th unit (0-based) of the chain u's tail starts, or -1.  Both chain
 * helpers are -called and defined in-file as stubs.
 */

#include "colonization.h"

extern int g_stub_a;
extern int top_unit(int u);
extern int next_unit(int u);

int nth_in_chain(int u, int n)
{
    register int index = -1;
    int found = -1;
    register int unit;

    for (unit = top_unit(u); found < 0 && unit >= 0; unit = next_unit(unit)) {
        index++;
        if (n == index)
            found = unit;
    }
    return found;
}
