/* rand_scaled_1070_ddfe: candidate for 1070:ddfe (45 bytes, 4 callers).  Verdict: RELOCATABLE.
 *
 * rand() scaled to 0..n-1 in 32-bit arithmetic: rand * n / 32768. The module-1070 copy of rand_scaled.
 */

#include "colonization.h"

int rand_scaled_1070_ddfe(int n)
{
    return (long)rand_rtl() * n / 0x8000L;
}
