/* alloc_slot: candidate for 1068:b75a (79 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 79, not the census 73. The trailing 6 bytes are the dead second epilogue.
 *
 * Takes the first of four slots that is not in use, clears its saved state and selects it;
 * returns the index, or -1 when all four are taken -- and calls `select_slot(-1)` in that
 * case too, which deselects.
 *
 * The in-use flags are bytes at DGROUP:0x56a0 and the states are the longs at 0x56a4 that
 * `select_slot` and `set_current_slot_state` already use, four bytes each. Both are reached
 * DS-relative.
 *
 * The loop stops on `found < 0 && i < 4` -- the search condition is in the `for`, not a
 * `break`, which is why the shipped tests DI before SI every time round.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` */

int alloc_slot(void)
{
    register int i;
    register int found;

    found = -1;
    for (i = 0; found < 0 && i < 4; i++) {
        if (g_slot_used[i] == 0) {
            g_slot_used[i] = 1;
            g_slot_state[i] = 0L;
            found = i;
        }
    }
    select_slot(found);
    return found;
}
