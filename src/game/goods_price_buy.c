/* goods_price_buy -- shipped at 1048:406e.
 *
 * Ported by tools/port.py from matched/game/goods_price_buy.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *   extern GoodsInfo far g_goods_info[];                 SEG20:0xd7a0
 *
 * The file's own Nation, GoodsInfo dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1048_dd61(col_i16 a, col_i16 b);

/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 goods_price_buy(col_i16 g)
{
    col_i16 p;

    p = ((signed char *)((unsigned char *)(g_cur_nation_rec) + 0x4c))[g];
    p = g_goods_info[g].spread + p;
    p = max_int_1048_dd61(0, p);
    return p;
}
