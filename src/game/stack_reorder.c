/* stack_reorder -- shipped at 1038:200f.
 *
 * Ported by tools/port.py from matched/game/stack_reorder.c.
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
col_i16 unit_at(col_i16 x, col_i16 y);
col_i16 unit_chain_next(col_i16 u);
col_i16 unit_chain_tail(col_i16 u);
void unit_detach_attach(col_i16 u, col_i16 a, col_i16 b);

#include "units.h"
/* 14 bytes at SEG20:0x7ac8 */



/* same object: 4-byte `push cs; call ` */





void stack_reorder(register col_i16 u)
{
    col_i16 next;
    col_i16 x;
    col_i16 y;
    col_i16 k;
    col_i16 last;

    last = -1;
    if (u < 0)
        goto out;
    x = g_units[u].x;
    y = g_units[u].y;
    for (u = unit_chain_tail(u); u >= 0; u = next) {
        next = unit_chain_next(u);
        if (g_unit_types[g_units[u].type].holds > 0) {
            last = u;
            unit_detach_attach(u, -3, -3);
        }
    }
    for (u = unit_at(x, y); u >= 0; u = next) {
        next = unit_chain_next(u);
        if (g_units[u].type == UNIT_TREASURE_TRAIN) {
            last = u;
            unit_detach_attach(u, -3, -3);
        }
    }
    for (k = 6; k >= 1; k--) {
        for (u = unit_at(x, y); u >= 0; u = next) {
            next = unit_chain_next(u);
            if (g_unit_types[g_units[u].type].size == (unsigned char)k) {
                last = u;
                unit_detach_attach(u, -3, -3);
            }
        }
    }
    if (last >= 0)
        for (u = unit_chain_tail(last); u >= 0; u = next) {
            next = unit_chain_next(u);
            unit_detach_attach(u, x, y);
        }
out:
    ;
}
