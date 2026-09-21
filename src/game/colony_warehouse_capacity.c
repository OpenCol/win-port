/* colony_warehouse_capacity -- shipped at 1018:46e7.
 *
 * Ported by tools/port.py from matched/game/colony_warehouse_capacity.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 colony_warehouse_capacity(void)
{
    col_i16 cap = 100;

    if (g_cur_colony_rec->warehouses != 0)
        cap += g_cur_colony_rec->warehouses * 100;
    return cap;
}
