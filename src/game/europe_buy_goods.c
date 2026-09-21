/* europe_buy_goods -- shipped at 1048:516b.
 *
 * Ported by tools/port.py from matched/game/europe_buy_goods.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 goods_price_buy(col_i16 g);
col_i16 min_int_1048_db44(col_i16 a, col_i16 b);
void market_record_purchase(col_i16 good, col_i16 n);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void nation_take_gold(col_i16 nation, col_i32 amount);
extern void unit_add_cargo(col_i16 unit, col_i16 good, col_i16 n);


/* same object: 4-byte `push cs; call ` */



col_i16 europe_buy_goods(col_i16 unit, col_i16 good, col_i16 want)
{
    col_i16 n;
    col_i16 cost;

    n = min_int_1048_db44(want, 100);
    cost = n * goods_price_buy(good);
    nation_take_gold(g_cur_nation, (col_i32)cost);
    market_record_purchase(good, n);
    unit_add_cargo(unit, good, n);
    return cost;
}
