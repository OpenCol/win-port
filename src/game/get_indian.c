/* get_indian: candidate for 1018:416d (114 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * For a valid colony tile slot (field_index >= 0), bit 4 of the plane-1 byte at the tile
 * (x - 2, y - 2) relative to the current colony's position; 0 otherwise.
 *
 * RENAMED 2026-09-26 from colony_tile_plane1_bit4: the Mac build's get_indian(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller colony_indian_table; callee feature_get; callee field_index), the runner-up gets 1, and the arity agrees (2).
 */

#include "colonization.h"

#include "terrain.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int field_index(int x, int y);
extern unsigned char feature_get(int x, int y);

int get_indian(int x, int y)
{
    int r = 0;
    int ring;

    ring = field_index(x, y);
    if (ring < 0)
        goto done;
    x += g_cur_colony_rec->x - 2;
    y += g_cur_colony_rec->y - 2;
    r = feature_get(x, y) & MAP1_CLAIM_SETTLED;
done:
    return r;
}
