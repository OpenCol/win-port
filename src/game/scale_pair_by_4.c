/* scale_pair_by_4: candidate for 1038:af4a (38 bytes).  Verdict: EXACT.
 *
 * Multiplies both arguments by four into two locals and returns neither: what
 * this computed for is gone, the arithmetic is not.
 */

#include "colonization.h"

void scale_pair_by_4(int a, int b)
{
    int x = a * 4;
    int y = b * 4;
}
