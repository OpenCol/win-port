/* get_production_class: candidate for 1018:82be (81 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Classifies a colony construction item: 0..0x29 are buildings (kind 1, index
 * as is), 0x2a..0x30 are units (kind 2, unit type item - 0x2a + 11);
 * anything else is kind 0.  The index goes to *idx when given.
 *
 * RENAMED 2026-09-26 from build_item_kind: the Mac build's get_production_class(short, short*), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller eligible_to_build; caller get_production_name; caller new_turn_colony; caller produce_something; caller resolve_raid), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

int get_production_class(int item, int *idx)
{
    int kind = 0;
    int i;

    if (item < 0) {
        kind = 0;
        i = 0;
    } else if (item < 0x2a) {
        kind = 1;
        i = item;
    } else {
        i = item - 0x2a;
        if (i < 7) {
            kind = 2;
            i += 11;
        }
    }
    if (idx != 0)
        *idx = i;
    return kind;
}
