/* base_defenders: candidate for 1030:8184 (109 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * size-source: 109, not the census 104. The trailing 5 bytes are the dead second epilogue
 *   that -Ot leaves after the return's own copy.
 * symbol: base_defenders
 * name-source: sibling-build
 *
 * CORRECTED 2026-09-29: this is the module's own `-Od -Ot` (it was recorded as `-O`, the only
 * `-O` file in the corpus), and the doubling is a SHIFT: `((level + 1) << 1) + 1`. Under -Ot
 * `(level + 1) * 2 + 1` folds to `level * 2 + 3`, which the shipped code does not do; `-O` kept
 * the arithmetic but dropped the dead epilogue, which the grader could not see at 104 bytes.
 *
 * The value the settlement-creation path stores at +4 of a new settlement: twice the
 * count in the 0x4e-byte table plus three, and the count plus one more again when bit 2 of the
 * settlement's own flags (+3) is set.  The 0x4e-byte table is indexed by the tribe's kind less four.
 *
 * RENAMED 2026-09-26 from settle_initial_size: the Mac build's base_defenders(short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in create_village -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

int base_defenders(int i)
{
    int tribe = g_settlements[i].owner - 4;
    int r = ((g_tribes[tribe].level + 1) << 1) + 1;
    if (g_settlements[i].flags & 4)
        r += g_tribes[tribe].level + 1;
    return r;
}
