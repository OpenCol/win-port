/* get_production_name: candidate for 1018:830f (168 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: get_production_name
 *
 * The TEXT string naming a construction item: the building's or the unit
 * type's name id, or the fallback id at SEG20:0xdd08 for anything else.
 * get_production_class is -called and defined in-file.
 *
 * RENAMED 2026-09-26 from build_item_name: the Mac build's get_production_name(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller DrawColonyUnits; caller new_turn_colony; caller produce_something; callee strings), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "misc.h"
#include "records.h"

extern char *strings(int n);

extern int get_production_class(int item, int *idx);

char *get_production_name(int item)
{
    int idx;
    int kind;
    char *s = 0;

    kind = get_production_class(item, &idx);
    switch (kind) {
    case 1:
        s = strings(g_buildings[idx].name);
        break;
    case 2:
        s = strings(g_unit_types[idx].name);
        break;
    case 0:
    default:
        s = strings(g_misc_words[MISC_NOTHING]);
        break;
    }
    return s;
}
