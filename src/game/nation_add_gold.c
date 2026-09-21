/* nation_add_gold -- shipped at 1018:4053.
 *
 * Ported by tools/port.py from matched/game/nation_add_gold.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern long g_stub_l;                                
 *
 * The file's own Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i32 max_long_1018_b375(col_i32 a, col_i32 b);
col_i32 min_long_1018_b3a3(col_i32 a, col_i32 b);
col_i32 nation_gold(col_i16 n);

/* 316 bytes at SEG20:0xc2f4 */







void nation_add_gold(col_i16 n, col_i32 amount)
{
    col_i32 v;

    v = nation_gold(n) + amount;
    v = max_long_1018_b375(v, 0L);
    v = min_long_1018_b3a3(v, 999999L);
    g_nations[n].gold = v;
}
