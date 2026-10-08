/* PORT: the links are 16-bit: swap (int *, 4 bytes here) would exchange the neighbouring field too; swap16 exchanges the word (W4). */
/* unit_swap_links: candidate for 1038:1f11 (254 bytes).  Verdict: RELOCATABLE.
 * symbol: unit_swap_links
 *
 * Swaps units a and b in their chain: a and b themselves are swapped when
 * a is found walking from b, then adjacent units (a's link2 == b) are
 * spliced, others exchange link and link2.  The chain step is -called
 * and defined in-file as a stub.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;
extern void swap(int *a, int *b);

extern int next_unit(int u);

static void swap16(short *a, short *b) { short t = *a; *a = *b; *b = t; }

void unit_swap_links(int unit_a, int unit_b)
{
    register int u;

    for (u = unit_b; u >= 0; u = next_unit(u)) {
        if (u == unit_a) {
            swap(&unit_a, &unit_b);
            u = -1;
        }
    }
    if (g_units[unit_a].link2 == unit_b) {
        g_units[unit_a].link2 = g_units[unit_b].link2;
        g_units[unit_b].link = g_units[unit_a].link;
        g_units[unit_b].link2 = unit_a;
        g_units[unit_a].link = unit_b;
    } else {
        swap16(&g_units[unit_a].link, &g_units[unit_b].link);
        swap16(&g_units[unit_a].link2, &g_units[unit_b].link2);
    }
}
