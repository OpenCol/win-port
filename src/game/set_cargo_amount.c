/* set_cargo_amount: candidate for 1018:7c81 (36 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Stores the amount of cargo slot `slot` of unit `u` (Unit+0x10 byte array).
 *
 * RENAMED 2026-09-26 from unit_set_cargo_amount: the Mac build's set_cargo_amount(short, short, short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in unload_cargo -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (3).
 */

#include "colonization.h"

#include "records.h"

void set_cargo_amount(int u, int slot, int amt)
{
    g_units[u].hold.amount[slot] = amt;
}
