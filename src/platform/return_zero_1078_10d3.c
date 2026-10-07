/* return_zero_1078_10d3: candidate for 1078:10d3 (20 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Returns zero and does nothing else -- `XOR AX,AX` between the prologue and the
 * epilogue. One of EIGHT copies in the image.
 *
 * 20 bytes, not the census 16: the `return 0;` is written out, which costs a second,
 * unreachable epilogue (rule 44). The body reads no argument, so it says nothing about
 * the parameter list (§119).
 */

#include "colonization.h"

int return_zero_1078_10d3(void)
{
    return 0;
}
