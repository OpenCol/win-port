/* get_progress: candidate for 1018:49ec (77 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The 4-bit value kept for colonist c in the packed nibble array at
 * Colony+0x60 (two colonists to a byte), 0 for c past the population.
 *
 * RENAMED 2026-09-26 from colonist_spec_nibble: the Mac build's get_progress(short), bound by call-graph agreement: 2 of its Mac call edges already bound land on this function (caller delete_colonist; caller new_turn_colony), the runner-up gets 1; and by call-sequence alignment: in delete_colonist the calls either side align and this is the one unbound callee left in that gap (1.0 votes, next 0.0); the arity agrees (1), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "records.h"

int get_progress(int colonist)
{
    int r;

    if (g_cur_colony_rec->pop > colonist) {
        r = g_cur_colony_rec->teach_progress[colonist >> 1];
        if (colonist & 1)
            r = r >> 4;
        r = r & 0xf;
    } else {
        r = 0;
    }
    return r;
}
