/* ai_sell: candidate for 1018:9400 (118 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: ai_sell
 *
 * Whether the AI may sell a GOODS kind from a colony (new_turn_colony passes its goods loop
 * index, as the alternative to custom_sell): never food, lumber, horses, tools or muskets, and
 * not ore while the colony has an armory or makes tools or muskets; 1 otherwise. (The header
 * and parameter read "building" until 2026-09-27.)
 * has_building is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from building_is_useful: the Mac build's ai_sell(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between colony_fortress and new_turn_colony, which are already bound; the arity agrees and the Mac body is 0.90x this one.
 */

#include "colonization.h"

#include "building.h"
#include "goods.h"

extern int g_stub_a;

extern int has_building(int bit);

int ai_sell(int goods)
{
    int r = 0;

    if (goods == GOODS_FOOD)
        goto done;
    if (goods == GOODS_LUMBER)
        goto done;
    if (goods == GOODS_HORSES)
        goto done;
    if (goods == GOODS_TOOLS)
        goto done;
    if (goods == GOODS_MUSKETS)
        goto done;
    if (goods == GOODS_ORE) {
        if (has_building(BUILDING_ARMORY) != 0)
            goto done;
        if (g_prod[GOODS_TOOLS] != 0)
            goto done;
        if (g_prod[GOODS_MUSKETS] != 0)
            goto done;
    }
    r = 1;
done:
    return r;
}
