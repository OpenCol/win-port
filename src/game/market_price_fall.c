/* market_price_fall -- shipped at 1048:4f20.
 *
 * Ported by tools/port.py from matched/game/market_price_fall.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *
 * The file's own Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1048_dd61(col_i16 a, col_i16 b);

/* same object: 4-byte `push cs; call ` (rule 47) */

void market_price_fall(register col_i16 g)
{
    col_i16 p;

    p = ((signed char *)((unsigned char *)(g_cur_nation_rec) + 0x4c))[g];
    p--;
    p = max_int_1048_dd61(0, p);
    ((signed char *)((unsigned char *)(g_cur_nation_rec) + 0x4c))[g] = (signed char)p;
}
