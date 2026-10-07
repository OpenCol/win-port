/* call_far_slot: candidate for 1070:9d56 (39 bytes).  Verdict: EXACT.
 *
 * Calls the function pointer the slot holds with two arguments, when
 * it is set.
 */

#include "colonization.h"

typedef void (*Handler)(int a, int b);

void call_far_slot(Handler *slot, int x, int y)
{
    if (*slot != 0)
        (*slot)(x, y);
}
