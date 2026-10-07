/* return_zero_1008_b239: candidate for 1008:b239 (20 bytes).  Verdict: EXACT.
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

int return_zero_1008_b239(void *unused1, void *unused2, void *unused3)
{
    return 0;
}
