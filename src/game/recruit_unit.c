/* recruit_unit -- shipped at 1028:3402.
 *
 * Ported by tools/port.py from matched/game/recruit_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Recruit far g_recruit[];                      SEG20:0xd83d
 *   extern Nation far * far g_cur_nation_rec;            DGROUP:0x4cc4
 *   extern int far g_turn_nation;                        SEG20:0x7796
 *
 * The file's own Unit, Recruit, Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 return_one_1028_33e9(col_i16 kind);

#include "units.h"
/* 6 bytes */
extern col_i16 create_unit_1038_2246(col_i16 type, col_i16 nation, col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` */


col_i16 recruit_unit(col_i16 kind, col_i16 a)
{
    col_i16 ok = 0;
    col_i16 price;
    register col_i16 u;

    if (return_one_1028_33e9(kind) == 0)
        goto out;
    price = g_recruit[kind].price;
    if ((col_i32)price > g_cur_nation_rec->gold)
        goto out;
    u = create_unit_1038_2246(g_recruit[kind].type, g_turn_nation,
                              g_turn_nation - 20, g_turn_nation - 20);
    if (u < 0)
        goto out;
    g_units[u].dest_x = g_cur_nation_rec->start_x;
    g_units[u].dest_y = g_cur_nation_rec->start_y;
    g_units[u].orders = (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) ? 0 : 1;
    g_cur_nation_rec->gold -= price;
    ok = 1;
out:
    return ok;
}
