/* select_slot: candidate for 1068:b70c (78 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * Switches the current slot: saves the outgoing slot's state into a per-slot table of
 * longs (one dword push, so not a pointer) when one is current, records the new index, and restores its state when valid.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void select_slot(int n)
{
    int i = n;

    if (g_current_slot == i)
        goto done;
    if (g_current_slot >= 0)
        g_slot_state[g_current_slot] = random_get_seed();
    g_current_slot = i;
    if (i >= 0)
        random_set_seed(g_slot_state[i]);
done:
    ;
}
