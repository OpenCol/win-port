/* refresh_three_indicators: candidate for 1010:e1c6 (159 bytes).  Verdict: RELOCATABLE.
 *
 * Saves and clears three status bytes, recomputes them, and for each one that went from
 * set to clear calls its handler with 1; a cleared status also resets its partner byte
 * to 0xfe.
 */

#include "colonization.h"

extern void draw_market(int n);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void draw_colony_map(int n);
void refresh_three_indicators(void)
{
    unsigned char a, b, c;

    a = g_st_a;
    g_st_a = 0;
    b = g_st_b;
    g_st_b = 0;
    c = g_st_c;
    g_st_c = 0;
    empty_1010_e1aa();
    if (g_st_a == 0 && a != 0)
        empty_1010_be3e(1);
    if (g_st_b == 0 && b != 0)
        draw_market(1);
    if (g_st_c == 0 && c != 0)
        draw_colony_map(1);
    if (g_st_a == 0)
        g_val_a = 0xfe;
    if (g_st_b == 0)
        g_val_b = 0xfe;
    if (g_st_c == 0)
        g_val_c = 0xfe;
}
