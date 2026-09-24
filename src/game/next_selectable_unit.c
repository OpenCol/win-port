/* next_selectable_unit -- shipped at 1038:3acb.
 *
 * Ported by tools/port.py from matched/game/next_selectable_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_selectable_or_in_europe(col_i16 c);

col_i16 next_selectable_unit(col_i16 c)
{
    col_i16 r;
    col_i16 last;

    if (g_unit_count <= c || c < 0) {
        c = -1;
        last = g_unit_count - 1;
        if (g_unit_count == 0)
            goto done;
    } else {
        last = c;
    }
    do {
        c++;
        if (g_unit_count <= c)
            c = 0;
        r = unit_selectable_or_in_europe(c);
    } while (r == 0 && c != last);
    if (r == 0)
        c = -1;
done:
    return c;
}
