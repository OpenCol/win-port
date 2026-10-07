/* get_field: candidate for 1018:423b (69 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: get_field
 *
 * The colonist working the colony-grid cell (x, y): the work-slot byte
 * (Colony+0x70) for the ring index of that cell, -1 when the cell is not a
 * ring tile.  field_index is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from colony_cell_colonist: the Mac build's get_field(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller ColonyMouseMap; caller DrawColonyMap; caller colony_indian_table; caller indian_supply_and_demand), the runner-up gets 1, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

extern int field_index(int x, int y);

char get_field(int x, int y)
{
    char r = -1;
    register int i;

    i = field_index(x, y);
    if (i >= 0)
        r = g_cur_colony_rec->slots[i];
    return r;
}
