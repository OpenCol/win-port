/* swap_word_identity: candidate for 1008:d739 (21 bytes, 98 call sites in 10 functions).  Verdict: EXACT.
 *   MOV AX,[BP+6]  -- returns the first argument, which for a function sits at
 *   BP+6 (BP+0 saved BP, BP+2..5 return address). A pure swap_word_identity/identity.
 */

#include "colonization.h"

int swap_word_identity(int a)
{
    return a;
}
