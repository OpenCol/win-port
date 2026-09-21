/* market_price_rise -- shipped at 1048:4ef7.
 *
 * Ported by tools/port.py from matched/game/market_price_rise.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *
 * The file's own Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void market_price_rise(col_i16 g)
{
    col_i16 p;

    p = ((signed char *)((unsigned char *)(g_cur_nation_rec) + 0x4c))[g];
    p++;
    ((signed char *)((unsigned char *)(g_cur_nation_rec) + 0x4c))[g] = (signed char)p;
}
