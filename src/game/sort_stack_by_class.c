/* sort_stack_by_class: candidate for 1038:200f (374 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * PUTS THE UNITS ON u'S SQUARE BACK IN DISPLAY ORDER. Units are lifted onto a holding chain at
 * (-3, -3) in passes, each pass taking what is left on the square: first every unit whose type
 * has cargo holds (+7 of the 14-byte type record), then every unit of type 0xa, then every unit
 * whose type byte at +8 is 6, 5, 4, 3, 2 and finally 1. Anything else stays where it is. Then
 * the holding chain, from the last unit lifted, is walked back onto the square.
 *
 * Each walk takes the next link BEFORE moving the current unit, since moving it rewrites the
 * link -- `next` is read first in every loop. The parameter `u` is reused as the walk variable.
 *
 * `size == (unsigned char)k`: the shipped compares `AL` against the low byte of `k`, where the
 * uncast int compare would widen the field first. `holds > 0` is the unsigned `JNA`. `u` is a
 * `register` parameter and keeps SI; `next` is the plain local promoted to DI after it
 * (rule 53). The four helpers come through the 4-byte `PUSH CS` form and are stubs (rule 47).
 *
 * RENAMED 2026-09-26 from stack_reorder: the Mac build's sort_stack_by_class(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller Stats; caller attempt_to_set_sail; caller orders_activate; callee next_unit; callee top_unit_in_square), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` */
extern int top_unit(int u);
extern int next_unit(int u);
extern void relocate_unit(int u, int x, int y);
extern int top_unit_in_square(int x, int y);

void sort_stack_by_class(register int u)
{
    int next;
    int x;
    int y;
    int unit_size;
    int last;

    last = -1;
    if (u < 0)
        goto out;
    x = g_units[u].x;
    y = g_units[u].y;
    for (u = top_unit(u); u >= 0; u = next) {
        next = next_unit(u);
        if (g_unit_types[g_units[u].type].holds > 0) {
            last = u;
            relocate_unit(u, -3, -3);
        }
    }
    for (u = top_unit_in_square(x, y); u >= 0; u = next) {
        next = next_unit(u);
        if (g_units[u].type == UNIT_TREASURE_TRAIN) {
            last = u;
            relocate_unit(u, -3, -3);
        }
    }
    for (unit_size = 6; unit_size >= 1; unit_size--) {
        for (u = top_unit_in_square(x, y); u >= 0; u = next) {
            next = next_unit(u);
            if (g_unit_types[g_units[u].type].size == (unsigned char)unit_size) {
                last = u;
                relocate_unit(u, -3, -3);
            }
        }
    }
    if (last >= 0)
        for (u = top_unit(last); u >= 0; u = next) {
            next = next_unit(u);
            relocate_unit(u, x, y);
        }
out:
    ;
}
