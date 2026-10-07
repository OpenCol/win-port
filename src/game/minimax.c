/* minimax: candidate for 1040:7ae2 (52 bytes).  Verdict: RELOCATABLE.
 *
 * Clamps v into [lo, hi] through the module's max and min helpers.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

int minimax(int value, int lo, int hi)
{
    value = max_int_1040_aab7(value, lo);
    value = min_int_1040_aadd(value, hi);
    return value;
}
