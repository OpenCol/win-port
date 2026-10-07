/* set_current_slot_state: candidate for 1068:b7a9 (55 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Replaces the current slot's saved state and re-applies it: deselect with
 * `select_slot(-1)`, which is what writes the outgoing state into the table, then overwrite
 * the entry that just got written, then reselect. Nothing happens when no slot is current.
 *
 * The two globals `select_slot` names are pinned here: `g_current_slot` is DGROUP:0x3246
 * and `g_slot_state` is DGROUP:0x56a4, four bytes per slot and stored with a 32-bit MOV.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` */

void set_current_slot_state(long state)
{
    register int cur;

    cur = g_current_slot;
    if (cur >= 0) {
        select_slot(-1);
        g_slot_state[cur] = state;
        select_slot(cur);
    }
}
