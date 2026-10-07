/* additional_spotting: candidate for 1038:2b17 (53 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: additional_spotting
 *
 * ORs `f` into the flags byte (Unit+3) of every unit from u along the
 * guarded chain.  The chain step is -called and defined in-file as a
 * stub.
 *
 * RENAMED 2026-09-26 from unit_chain_set_flags: the Mac build's additional_spotting(short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between in_view and get_spotting, which are already bound; the arity agrees and the Mac body is 1.58x this one.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

extern int next_unit(int u);

void additional_spotting(int u, int bits)
{
    while (u >= 0) {
        g_units[u].flags |= bits;
        u = next_unit(u);
    }
}
