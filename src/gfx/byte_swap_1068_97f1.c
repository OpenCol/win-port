/* byte_swap_1068_97f1: candidate for 1068:97f1 (40 bytes).  Verdict: EXACT.
 *
 * Swaps the two halves of a word.  The right shift is `SHR`, not `SAR`, so
 * the value is unsigned in the source.
 */

#include "colonization.h"

unsigned byte_swap_1068_97f1(unsigned v)
{
    return ((v & 0xff) << 8) | ((v & 0xff00) >> 8);
}
