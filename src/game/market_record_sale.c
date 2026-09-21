/* market_record_sale -- shipped at 1048:506e.
 *
 * Ported by tools/port.py from matched/game/market_record_sale.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *   extern GoodsRec far g_goods[];                       SEG20:0xd79c
 *
 * The file's own Nation, GoodsRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 goods_price_sell(col_i16 g);
col_i16 scaled_rate(col_i16 amount);

#include "nations.h"
/* 316 bytes at SEG20:0xc2f4 */


/* same object: 4-byte `push cs; call ` */



void market_record_sale(register col_i16 good, col_i16 amount)
{
    col_i16 base;
    col_i16 add;
    col_i16 price;
    col_i16 gross;
    col_i32 net;
    register col_i16 n;

    base = scaled_rate(amount) + (amount << g_goods[good].shift);
    for (n = 0; n < NATION_EUROPEAN_COUNT; n++) {
        add = (n == 3) ? base * 2 / 3 : base;
        ((col_i16 *)((unsigned char *)&(g_nations[n]) + 0x5c))[good] += add;
    }
    ((col_i32 *)((unsigned char *)(g_cur_nation_rec) + 0xbc))[good] += amount;
    ((col_i32 *)((unsigned char *)(g_cur_nation_rec) + 0xfc))[good] += amount;
    price = goods_price_sell(good);
    gross = price * amount;
    net = (col_i32)gross * (100L - g_cur_nation_rec->tax) / 100;
    ((col_i32 *)((unsigned char *)(g_cur_nation_rec) + 0x7c))[good] += net;
}
