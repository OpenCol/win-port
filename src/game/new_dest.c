/* new_dest: candidate for 1028:c5ca (46 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: new_dest
 *
 * Initialises the current trade route's entry: stores the caller's value in its first
 * field and clears both nibbles of the packed byte at +2. Called from route_create.
 *
 * RENAMED 2026-09-26 from route_entry_init: the Mac build's new_dest(short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in route_create -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void set_num_orders(int which, int count);
void new_dest(int colony)
{
    g_cur_route_stop->colony = colony;
    set_num_orders(1, 0);
    set_num_orders(0, 0);
}
