/* stack_cargo_room -- shipped at 1038:364f.
 *
 * Ported by tools/port.py from matched/game/stack_cargo_room.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_chain_next(col_i16 u);
col_i16 unit_chain_tail(col_i16 u);
void unit_unload_carrier(col_i16 u);

#include "units.h"
/* 14 bytes at SEG20:0x7ac8 */



/* same object: 4-byte `push cs; call ` */




col_i16 stack_cargo_room(col_i16 u, unsigned char need, col_i16 last_ship)
{
    col_i16 r;
    col_i16 n;
    col_i16 space;
    col_i16 size;
    col_i16 unplaced;
    unsigned char room[0x40];
    register col_i16 i;
    register col_i16 unit;

    r = 0;
    n = 0;
    if (last_ship != 0)
        unit_unload_carrier(u);
    for (unit = unit_chain_tail(u); unit >= 0; unit = unit_chain_next(unit)) {
        if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST) {
            space = g_unit_types[g_units[unit].type].holds - g_units[unit].cargo_count;
            room[n] = space;
            n++;
        }
    }
    for (unit = unit_chain_tail(u); unit >= 0; unit = unit_chain_next(unit)) {
        if (g_units[unit].type < UNIT_SHIP_FIRST || g_units[unit].type > UNIT_SHIP_LAST) {
            size = g_unit_types[g_units[unit].type].size;
            if (size < 0x63) {
                unplaced = 1;
                for (i = 0; unplaced != 0 && i < n; i++) {
                    if (room[i] >= (unsigned char)size) {
                        room[i] -= (unsigned char)size;
                        unplaced = 0;
                    }
                }
                if (unplaced != 0)
                    goto out;
            }
        }
    }
    if (n == 0)
        goto out;
    if (last_ship != 0) {
        if (room[n - 1] >= need)
            r = room[n - 1];
    } else {
        for (i = 0; r == 0 && i < n; i++)
            if (room[i] >= need)
                r = room[i];
    }
out:
    return r;
}
