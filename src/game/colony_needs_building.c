/* colony_needs_building -- shipped at 1018:038f.
 *
 * Ported by tools/port.py from matched/game/colony_needs_building.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern BuildNeed far g_build_needs[];                SEG20:0xdca8
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern unsigned int far g_prod[];                    SEG21:0x0122
 *
 * The file's own BuildNeed, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 4 bytes at SEG20:0xdca8 */

/* 0xca bytes */


extern col_i16 current_colony_building_count(col_i16 b);

col_i16 colony_needs_building(col_i16 idx)
{
    register col_i16 need = 0;
    register col_i16 goods = g_build_needs[idx].goods;
    col_i16 b = g_build_needs[idx].building;
    col_i16 have = current_colony_building_count(b);

    if (g_prod[goods] >= 3)
        need = 2;
    if (g_prod[goods] >= 8)
        need = 3;
    if (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[goods] >= 100)
        need = 3;
    return have < need;
}
