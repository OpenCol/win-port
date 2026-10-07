/* get_spotting: candidate for 1038:2b4c (116 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: get_spotting
 *
 * Bits 4..7 for the nations present at (x, y): the plane-2 high nibble's
 * owner when `nation` is European (< 4), plus every nation whose
 * in_view test passes.  The latter is -called and defined
 * in-file as a stub.
 *
 * RENAMED 2026-09-26 from tile_nation_bits: the Mac build's get_spotting(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller move_unit; callee in_view; callee owner_of), the runner-up gets 2, the arity agrees (3), and the Mac body is 0.98x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour.
 */

#include "colonization.h"

#include "units.h"

extern int g_stub_a;
extern char owner_of(int x, int y);

extern int in_view(int x, int y, int nation);

int get_spotting(int x, int y, int nation)
{
    register int bits = 0;
    int i;

    if (nation < 4) {
        i = owner_of(x, y);
        bits |= UNIT_SEEN_BY(i);
    }
    for (i = 0; i < 4; i++) {
        if (in_view(x, y, i) != 0)
            bits |= UNIT_SEEN_BY(i);
    }
    return bits;
}
