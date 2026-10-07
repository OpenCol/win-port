/* wrap_index: candidate for 1040:7d49 (41 bytes).  Verdict: EXACT.
 *
 * Wraps an index into 0..n-1: a negative index becomes n - 1, one at or
 * past n becomes 0.
 */

#include "colonization.h"

int wrap_index(int i, int n)
{
    if (i < 0)
        i = n - 1;
    if (i >= n)
        i = 0;
    return i;
}
