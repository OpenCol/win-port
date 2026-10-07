/* test_check_mask: candidate for 1050:63fb (30 bytes).  Verdict: RELOCATABLE.
 *
 * Bit n of the flag word at DGROUP:0x2514, returned as the masked value.
 */

#include "colonization.h"

int test_check_mask(int n)
{
    return (1 << n) & g_check_mask;
}
