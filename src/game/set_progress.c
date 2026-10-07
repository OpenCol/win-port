/* set_progress: candidate for 1018:4a39 (116 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: set_progress
 *
 * Stores v (capped at 15) as colonist c's nibble in the packed array at
 * Colony+0x60; nothing for c past the population.  min_int is -called
 * and defined in-file.
 *
 * RENAMED 2026-09-26 from colonist_set_spec_nib: the Mac build's set_progress(short, short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in colony_plan, new_turn_colony, set_job -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (2.5 votes, the next candidate 0.0); the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

void set_progress(int colonist, int progress)
{
    int mask;

    if (g_cur_colony_rec->pop > colonist) {
        mask = 0xf0;
        progress = min_int_1018_b329(progress, 15);
        if (colonist & 1) {
            mask = mask >> 4;
            progress = progress << 4;
        }
        g_cur_colony_rec->teach_progress[colonist >> 1] &= mask;
        g_cur_colony_rec->teach_progress[colonist >> 1] |= progress;
    }
}
