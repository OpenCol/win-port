/* rand_scaled: candidate for 1068:d840 (45 bytes, 5 callers).  Verdict: RELOCATABLE.
 *
 * rand() scaled to 0..n-1 in 32-bit arithmetic: rand * n / 32768. A copy lives at 1070:ddfe.
 */

#include "colonization.h"

int rand_scaled(int n)
{
    return (long)rand_rtl() * n / 0x8000L;
}
