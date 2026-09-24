/* wake_units_near -- shipped at 1030:4f7c.
 *
 * Ported by tools/port.py from matched/game/wake_units_near.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))
extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 square_european_nation(col_i16 x, col_i16 y);

void wake_units_near(col_i16 nation, col_i16 owner)
{
    col_i16 found;
    col_i16 x;
    col_i16 y;
    col_i16 nx;
    col_i16 ny;
    register col_i16 i;
    register col_i16 u;

    for (u = 0; g_unit_count > u; u++) {
        if (NATION(g_units[u]) != (unsigned char)owner)
            continue;
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
            continue;
        if (g_unit_types[g_units[u].type].attack <= 1)
            continue;
        x = g_units[u].x;
        y = g_units[u].y;
        found = 0;
        if (on_map(x, y) == 0)
            continue;
        for (i = 0; found == 0 && i < 8; i++) {
            nx = g_dir_dx[i] + x;
            ny = g_dir_dy[i] + y;
            if (square_european_nation(nx, ny) == nation)
                found = 1;
        }
        if (found != 0) {
            if (g_units[u].orders == ORDERS_FORTIFY || g_units[u].orders == ORDERS_FORTIFIED)
                g_units[u].orders = ORDERS_NONE;
        }
    }
}
