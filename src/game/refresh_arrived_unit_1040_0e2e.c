/* refresh_arrived_unit_1040_0e2e -- shipped at 1040:0e2e.
 *
 * Ported by tools/port.py from matched/game/refresh_arrived_unit_1040_0e2e.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern int g_1ccc;                                   DGROUP:0x1ccc
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void paint_unit(col_i16 u, col_i16 a, col_i16 b);

void refresh_arrived_unit_1040_0e2e(col_i16 flag)
{
    if (g_1ccc != 0) {
        paint_unit(g_selected_unit, 1, 0);
        if (flag != 0)
            paint_unit(g_selected_unit, 1, 1);
    } else {
        paint_unit(g_selected_unit, 0, 0);
    }
}
