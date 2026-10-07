/* get_production_cost: candidate for 1018:83b7 (197 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: get_production_cost
 *
 * The hammers a construction item costs, with its tools in *tools (x10): a
 * building's from BuildingRec (+0xa, +7), a unit's from its type (hammers
 * byte x32, raised to 39 or 52 minimum; tools byte x10).  0 for anything
 * else.  get_production_class is -called and defined in-file.
 *
 * RENAMED 2026-09-26 from build_item_cost: the Mac build's get_production_cost(short, short*), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between get_production_name and job_available, which are already bound; the arity agrees and the Mac body is 0.97x this one.
 */

#include "colonization.h"

#include "records.h"

extern int get_production_class(int item, int *idx);

int get_production_cost(int item, int *tools)
{
    register int hammers;
    register int tool_cost = 0;
    int idx;
    int kind;

    kind = get_production_class(item, &idx);
    switch (kind) {
    case 1:
        hammers = g_buildings[idx].hammers;
        tool_cost = g_buildings[idx].tools * 10;
        break;
    case 2:
        hammers = g_unit_types[idx].hammers << 5;
        if (hammers < 0x27)
            hammers = 0x27;
        else if (hammers < 0x34)
            hammers = 0x34;
        tool_cost = g_unit_types[idx].tools * 10;
        break;
    case 0:
    default:
        hammers = 0;
        break;
    }
    if (tools != 0)
        *tools = tool_cost;
    return hammers;
}
