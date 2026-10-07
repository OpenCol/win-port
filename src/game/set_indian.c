/* set_indian: candidate for 1018:41df (92 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: set_indian
 *
 * For a colony-grid position that is one of the ring offsets, sets or clears (per `bits`)
 * the plane-1 MAP1_CLAIM_SETTLED bit on the map tile it stands for (colony x/y - 2 + grid x/y).
 * field_index is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from ring_tile_set_bits: the Mac build's set_indian(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller set_field; caller try_to_set_job; callee feature_set; callee field_index), the runner-up gets 2, and the arity agrees (3).
 */

#include "colonization.h"

#include "terrain.h"
#include "records.h"

extern int g_stub_a;
extern void feature_set(int x, int y, int mask, int bits);

extern int field_index(int x, int y);

void set_indian(int x, int y, int bits)
{
    int i;

    i = field_index(x, y);
    if (i >= 0) {
        x += g_cur_colony_rec->x - 2;
        y += g_cur_colony_rec->y - 2;
        feature_set(x, y, MAP1_CLAIM_SETTLED, bits);
    }
}
