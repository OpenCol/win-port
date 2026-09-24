/* native_land_price -- shipped at 1038:0c18.
 *
 * Ported by tools/port.py from matched/game/native_land_price.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Settlement far * far g_cur_settlement_rec;    DGROUP:0x4c2c
 *   extern Tribe far * far g_cur_tribe_rec;              DGROUP:0x4c28
 *   extern int far g_cur_tribe;                          DGROUP:0x4c30
 *   extern unsigned char far g_nation_population[];      DGROUP:0x4b54
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern int far g_turn_nation;                        SEG20:0x7796
 *
 * The file's own Settlement, Tribe, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1038_86a6(col_i16 a, col_i16 b);
col_i16 quarter_of_100(col_i16 v);
col_i16 table_78_word(col_i16 t, col_i16 i);
void set_current_native_settlement(col_i16 s);

#include "nations.h"
#include "fathers.h"
#include "control.h"
#include "settle.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */







extern col_i16 octile_distance_xy(col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2);
extern col_i16 * max_ptr_1038_8d92(col_i16 *a, col_i16 *b);
extern col_i16 tile_decoration(col_i16 x, col_i16 y);
extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);

/* same object: 4-byte `push cs; call ` */





col_i16 native_land_price(col_i16 settlement, register col_i16 nation, col_i16 x, col_i16 y)
{
    col_i16 dist;
    col_i16 mult;
    col_i16 alarm;
    col_i16 zero;
    register col_i16 price;

    set_current_native_settlement(settlement);
    dist = octile_distance_xy(x, y, g_cur_settlement_rec->x, g_cur_settlement_rec->y);
    if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN) {
        price = g_cur_tribe_rec->level + g_difficulty * 2 + 6 - dist + g_cur_tribe_rec->count;
        mult = 0x41;
    } else {
        price = g_cur_tribe_rec->level + 0xc - dist - g_difficulty + g_cur_tribe_rec->count;
        mult = 0x32;
    }
    price = price - *max_ptr_1038_8d92((zero = 0, &zero),
                                       (alarm = (10 - g_nation_population[nation]) >> 1, &alarm));
    if (tile_decoration(x, y) != -1)
        price = price * 2;
    price = max_int_1038_86a6(1, price);
    price = price * mult;
    if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN)
        price = (quarter_of_100(table_78_word(g_cur_tribe, g_turn_nation)) + 1) * price;
    if (g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL)
        price += price >> 1;
    if (nation_flag_test(nation, FATHER_PETER_MINUIT) != 0)
        price = 0;
    price = price >> 1;
    return price;
}
