/* swap_word_identity_1068_9518: candidate for 1068:9518 (21 bytes, 98 call sites in 10 functions).  Verdict: EXACT.
 *   MOV AX,[BP+6]  -- returns the first argument, which for a function sits at
 *   BP+6 (BP+0 saved BP, BP+2..5 return address). A pure swap_word_identity_1068_9518/identity.
 *
 * TRANSFERRED: byte-identical to `swap_word_identity` (1008:d739), one of 3 copies of this
 * function in the image. Same source, this segment's copy.
 */

#include "colonization.h"

int swap_word_identity_1068_9518(int a)
{
    return a;
}
