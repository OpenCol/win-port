/* force_map: candidate for 1038:b1a0 (43 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * size-source: 43, not the census 37. The trailing 6 bytes are a SECOND epilogue, and
 * they are there because the body ends in an explicit `return;` -- Borland emits an
 * epilogue for the return statement AND one for the end of the function (rule 44).
 * symbol: force_map
 *
 * possibly_center over the one-tile rectangle (x, y)-(x, y) with flag 0.
 * The callee is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from redraw_tile: the Mac build's force_map(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller combat_fight; caller indian_move; caller negotiate_tribe; caller revolution_mobilize; caller unit_rules), the runner-up gets 3, and the arity agrees (2).
 */

#include "colonization.h"

extern int g_stub_a;

extern void possibly_center(int x1, int y1, int x2, int y2, int flag);

void force_map(int x, int y)
{
    possibly_center(x, y, x, y, 0);
    return;
}
