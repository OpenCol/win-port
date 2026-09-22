/* can_build_item -- shipped at 1018:8933.
 *
 * Ported by tools/port.py from matched/game/can_build_item.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern BuildingRec far g_buildings[];                SEG20:0xd422
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern RingRec g_ring_counts_1[];                    DGROUP:0x0c01
 *   extern signed char far g_ring_dx[];                  SEG27:0x0012
 *   extern signed char far g_ring_dy[];                  SEG27:0x0027
 *
 * The file's own Colony, BuildingRec, Unit, RingRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 build_item_kind(col_i16 item, col_i16 *idx);
col_i16 colony_ring_level(void);
col_i16 colony_warehouse_capacity(void);
col_i16 current_colony_flag_get(col_i16 bit);

#include "building.h"
#include "units.h"
#include "fathers.h"
/* 12 bytes at SEG20:0xd422 */
extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);

/* same object: 4-byte `push cs; call ` */





col_i16 can_build_item(col_i16 item)
{
    col_i16 req;
    col_i16 kind;
    col_i16 idx;
    col_i16 n;
    col_i16 found;
    col_i16 x;
    col_i16 y;
    register col_i16 ok;
    register col_i16 i;

    ok = 1;
    kind = build_item_kind(item, &idx);
    if (kind == 0)
        goto out;
    if (kind == 1) {
        ok = 1;
        if (g_buildings[idx].min_pop > g_cur_colony_rec->pop)
            ok = 0;
        req = g_buildings[idx].prev_level;
        if (req >= 0 && current_colony_flag_get(req) == 0)
            ok = 0;
        req = g_buildings[idx].f2;
        if (current_colony_flag_get(idx) != 0)
            ok = 0;
        if (idx == 6) {
            n = (g_ring_counts_1 - 1)[colony_ring_level()].count;
            found = 0;
            for (i = 0; found == 0 && i < n; i++) {
                x = g_cur_colony_rec->x + g_ring_dx[i];
                y = g_cur_colony_rec->y + g_ring_dy[i];
                if (on_map(x, y) == 0)
                    continue;
                if (ocean(x, y) != 0)
                    found = 1;
            }
            if (found == 0)
                ok = 0;
        }
        if (idx == 7 && !(g_cur_colony_rec->status & 0x40))
            ok = 0;
        if (idx == 0x12 && nation_flag_test(g_cur_colony_rec->nation, FATHER_PETER_STUYVESANT) == 0)
            ok = 0;
        if (idx == 0xa)
            ok = 0;
        if (idx == 0xb)
            ok = 0;
        if (idx == 0x1e)
            ok = 0;
        if (idx == 0x10 && colony_warehouse_capacity() >= 0x12c) {
            ok = 0;
            goto out;
        }
        if ((idx == 0x17 || idx == 0x1a || idx == 0x1d || idx == 0x22 || idx == 5 || idx == 0x29)
            && nation_flag_test(g_cur_colony_rec->nation, FATHER_ADAM_SMITH) == 0)
            ok = 0;
    }
    if (kind == 2) {
        ok = 1;
        if (idx >= 0xd && idx <= 0x12) {
            if (current_colony_flag_get(BUILDING_SHIPYARD) == 0)
                ok = 0;
        } else if (idx == 0xc) {
            ok = 1;
        } else if (idx == 0xb) {
            ok = current_colony_flag_get(BUILDING_ARMORY);
        } else if (current_colony_flag_get(BUILDING_LUMBER_MILL) == 0) {
            ok = 0;
        }
    }
    if (g_cur_colony_rec->item == (unsigned char)item) {
        if (g_units[item].type != UNIT_SOLDIERS)
            ok = 1;
        else if (current_colony_flag_get(idx) == 0)
            ok = 1;
    }
out:
    return ok;
}
