/* tribe_radius: candidate for 1038:00b2 (80 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Maps the tribe's kind byte at +2 of its 0x4e-byte record to a level: kinds 0 and 1 give 1, kind 2
 * gives 2, kind 3 (and any other value) 3.

 * SIZE: the census says 72 and the function is 80. The extra bytes are the CS-relative
 * jump table for this function's `switch`, which Borland puts in the CODE segment and
 * Ghidra's reachable-code size excludes. The reconstruction grades over the whole
 * extent unchanged; only the number moved.
 *
 * RENAMED 2026-09-26 from tribe_level: the Mac build's tribe_radius(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller colony_indian_table; caller orders_clear; caller orders_road; caller perform_orders_clear; caller perform_orders_road), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

int tribe_radius(int tribe)
{
    int radius;

    switch (g_tribes[tribe].level) {
    case 0: radius = 1; break;
    case 1: radius = 1; break;
    case 2: radius = 2; break;
    case 3:
    default: radius = 3; break;
    }
    return radius;
}
