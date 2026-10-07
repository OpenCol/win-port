/* nth_eligible_build: candidate for 1018:8be3 (82 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: nth_eligible_build
 *
 * The nth construction item (over -1..0x30) that eligible_to_build accepts; -2
 * when there are not that many.  The helper is -called and defined
 * in-file as a stub.
 *
 * RENAMED 2026-09-27 from nth_buildable: the Mac build's nth_eligible_build, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: The nth item that eligible_to_build accepts, or -2; Mac nth_eligible_build's only call is eligible_to_build; 1 arg, ratio 2.12.
 */

#include "colonization.h"

extern int g_stub_a;

extern int eligible_to_build(int item);

int nth_eligible_build(int nth)
{
    int r = -2;
    register int k = -1;
    register int i;

    for (i = -1; r < -1 && i < 0x31; i++) {
        if (eligible_to_build(i) != 0) {
            k++;
            if (k == nth)
                r = i;
        }
    }
    return r;
}
