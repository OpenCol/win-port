/* passenger: candidate for 1028:0aa7 (122 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 122, not the census 116. The trailing 6 bytes are a second epilogue.
 *
 * Walks the unit chain from `u` and returns the flagged unit with the highest type index,
 * or -1. "Flagged" is bit 0x40 of the last byte of the 14-byte unit-type record at
 * SEG20:0x7ac8 -- the same table `num_cargo_vessels` tests at +0 and `get_production_cost`
 * reads for hammers and tools. What the bit means is not settled here, so the name does
 * not claim it.
 *
 * `next_unit` is the chain step, as in `num_cargo_vessels` and
 * `pick_ship_in_chain`.
 *
 * The flag test is a positive `if` and NOT a `continue`: the shipped `JZ` is a direct
 * conditional jump over the block (rule 12), where a `continue` gives invert-and-jump-over.
 * The two tests inside it are one `||`, whose `JL` lands ON the assignment.
 *
 * RENAMED 2026-09-26 from chain_highest_flagged: the Mac build's passenger(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between unit_build_another and find_beach, which are already bound; the arity agrees and the Mac body is 1.07x this one.
 */

#include "colonization.h"

#include "records.h"

extern int next_unit(int u);

int passenger(register int u)
{
    int best = -1;

    for (; u >= 0; u = next_unit(u)) {
        if ((g_unit_types[g_units[u].type].bits & 0x40) != 0) {
            if (best < 0 || g_units[u].type > g_units[best].type)
                best = u;
        }
    }
    return best;
}
