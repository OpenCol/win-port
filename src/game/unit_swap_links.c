/* unit_swap_links -- shipped at 1038:1f11.
 *
 * Ported by tools/port.py from matched/game/unit_swap_links.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int g_stub_a;                                 
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_chain_next(col_i16 u);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */


extern void swap_ints(col_i16 *a, col_i16 *b);



void unit_swap_links(col_i16 a, col_i16 b)
{
    register col_i16 c;

    for (c = b; c >= 0; c = unit_chain_next(c)) {
        if (c == a) {
            swap_ints(&a, &b);
            c = -1;
        }
    }
    if (g_units[a].link2 == b) {
        g_units[a].link2 = g_units[b].link2;
        g_units[b].link = g_units[a].link;
        g_units[b].link2 = a;
        g_units[a].link = b;
    } else {
        swap_ints(&g_units[a].link, &g_units[b].link);
        swap_ints(&g_units[a].link2, &g_units[b].link2);
    }
}
