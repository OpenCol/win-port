/* round_up_4: candidate for 1070:dc61 (33 bytes).  Verdict: EXACT.
 *
 * Rounds up to a multiple of four, the DIB row padding rule.
 */

#include "colonization.h"

int round_up_4(int n)
{
    return (n + 3) / 4 * 4;
}
