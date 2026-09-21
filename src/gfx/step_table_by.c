/* step_table_by -- shipped at 1068:99e7.
 *
 * Ported by tools/port.py from matched/gfx/step_table_by.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far *g_scale_map;                         DGROUP:0x5694
 *   extern int g_scale_num;                              DGROUP:0x2b0e
 *   extern int g_scale_den;                              DGROUP:0x2b10
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 step_table_by(col_i16 a, col_i16 b, col_i16 exact)
{
    col_i16 limit;
    col_i16 neg;
    col_i16 i;

    if (a == 0) return 0;
    if (a < 0) {
        neg = 1;
        a = -a;
    } else
        neg = 0;
    if (g_scale_num < g_scale_den) {
        i = b;
        limit = g_scale_map[b] + a;
        while (g_scale_map[i] < limit)
            i++;
        if (exact == 0 && g_scale_map[i] != limit)
            i--;
    } else {
        i = b;
        limit = g_scale_map[b] + a;
        while (g_scale_map[i] < limit)
            i++;
    }
    return (neg ? -1 : 1) * (i - b);
}
