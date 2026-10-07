/* eligible_to_build: candidate for 1018:8933 (636 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * name-source: sibling-build
 * size-source: 636, not the census 627. The trailing 9 bytes are the dead second epilogue.
 *
 * WHETHER THE CURRENT COLONY MAY BUILD `item`. `get_production_class` splits it into a
 * building (kind 1) or a unit (kind 2); anything else answers 1 unchanged.
 *
 * A BUILDING needs a population at least its record's +8, its prerequisite at +3 already built,
 * and itself not built yet. Then the specials: the docks (6) need an ocean square among the
 * colony's own ring; 7 needs bit 6 of the colony byte at +0x1c; 0x12 needs nation flag 3;
 * 0xa, 0xb and 0x1e can never be built; 0x10 is refused once the warehouse holds 300; and
 * 0x17, 0x1a, 0x1d, 0x22, 5 and 0x29 all need nation flag 0.
 *
 * A UNIT needs the shipyard-class flag 8 for types 0xd..0x12, nothing for 0xc, colony flag 3 for
 * 0xb -- assigned straight through, so the flag's own value is the answer -- and flag 0x24 for
 * everything else.
 *
 * THE ITEM ALREADY UNDER CONSTRUCTION IS ALWAYS ALLOWED, whatever the rules above said: if the
 * colony's +0x94 names this item, the answer goes back to 1 unless `g_units[item].type` is 1 and
 * the colony already has flag `idx`. Indexing the UNIT array with a build item is what the
 * shipped does; it reads as a bug and is recorded rather than repaired.
 *
 * Two stores to `req` -- the record's +3, which is tested, and +2, which is dead -- are in the shipped and have to be
 * spelled out. `!(byte & 0x40)` loads and promotes before testing AL (rule 59), and the
 * colony-ring count is `g_ring_tile_count[level - 1]`, the same spelling
 * `colony_owns_tile` needed.
  *
 * THE NATION FLAGS ARE FOUNDING FATHERS (include/fathers.h). The custom house (0x12, BUILDING18) needs
 * FATHER_PETER_STUYVESANT, and the textile mill, cigar factory, rum factory, coat factory, arsenal and
 * iron works (0x17, 0x1a, 0x1d, 0x22, 5, 0x29) need FATHER_ADAM_SMITH -- each father's documented
 * effect, by the index of the game's own FATHER table.
 *
 * RENAMED 2026-09-26 from can_build_item: the Mac build's eligible_to_build(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (callee get_warehouse; callee has_building; callee has_father; callee on_map), the runner-up gets 1, and the arity agrees (1).
 */

#include "colonization.h"

#include "building.h"
#include "units.h"
#include "fathers.h"
#include "records.h"

extern int terrain_is_ocean(int x, int y);
extern int has_father(int nation, int bit);

/* same object: 4-byte `push cs; call ` */
extern "C" int get_production_class(int item, int *idx);
extern "C" int has_building(int bit);
extern "C" int colony_map_status(void);
extern "C" int get_warehouse(void);

int eligible_to_build(int item)
{
    int req;
    int kind;
    int idx;
    int n;
    int found;
    int x;
    int y;
    register int ok;
    register int i;

    ok = 1;
    kind = get_production_class(item, &idx);
    if (kind == 0)
        goto out;
    if (kind == 1) {
        ok = 1;
        if (g_buildings[idx].min_pop > g_cur_colony_rec->pop)
            ok = 0;
        req = g_buildings[idx].prev_level;
        if (req >= 0 && has_building(req) == 0)
            ok = 0;
        req = g_buildings[idx].prereq2;
        if (has_building(idx) != 0)
            ok = 0;
        if (idx == 6) {
            n = g_ring_tile_count[colony_map_status() - 1];
            found = 0;
            for (i = 0; found == 0 && i < n; i++) {
                x = g_cur_colony_rec->x + g_ring_dx[i];
                y = g_cur_colony_rec->y + g_ring_dy[i];
                if (on_map(x, y) == 0)
                    continue;
                if (terrain_is_ocean(x, y) != 0)
                    found = 1;
            }
            if (found == 0)
                ok = 0;
        }
        if (idx == 7 && !(g_cur_colony_rec->status & 0x40))
            ok = 0;
        if (idx == 0x12 && has_father(g_cur_colony_rec->nation, FATHER_PETER_STUYVESANT) == 0)
            ok = 0;
        if (idx == 0xa)
            ok = 0;
        if (idx == 0xb)
            ok = 0;
        if (idx == 0x1e)
            ok = 0;
        if (idx == 0x10 && get_warehouse() >= 0x12c) {
            ok = 0;
            goto out;
        }
        if ((idx == 0x17 || idx == 0x1a || idx == 0x1d || idx == 0x22 || idx == 5 || idx == 0x29)
            && has_father(g_cur_colony_rec->nation, FATHER_ADAM_SMITH) == 0)
            ok = 0;
    }
    if (kind == 2) {
        ok = 1;
        if (idx >= 0xd && idx <= 0x12) {
            if (has_building(BUILDING_SHIPYARD) == 0)
                ok = 0;
        } else if (idx == 0xc) {
            ok = 1;
        } else if (idx == 0xb) {
            ok = has_building(BUILDING_ARMORY);
        } else if (has_building(BUILDING_LUMBER_MILL) == 0) {
            ok = 0;
        }
    }
    if ((unsigned char)g_cur_colony_rec->item == (unsigned char)item) {
        if (g_units[item].type != UNIT_SOLDIERS)
            ok = 1;
        else if (has_building(idx) == 0)
            ok = 1;
    }
out:
    return ok;
}
