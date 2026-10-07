/* MSHideCursor: candidate for 1078:292f (27 bytes).  Verdict: RELOCATABLE.
 * symbol: MSHideCursor
 *
 * Hides the cursor for certain, not just once: Windows keeps a show COUNT, and one
 * ShowCursor(FALSE) only decrements it. This spins until the count goes negative, which is
 * the documented test for the cursor actually being hidden.
 *
 * The loop body is empty, so the `JMP` Borland emits to the bottom test lands on the
 * instruction after it -- a two-byte jump to the next instruction, which looks like dead
 * code and is just what `while (cond) ;` compiles to at `-Od`.
 */

#include "colonization.h"

void MSHideCursor(void)
{
    while (ShowCursor(0) >= 0)
        ;
}
