/* colonist_at_cell -- shipped at 1018:6316.
 *
 * Ported by tools/port.py from matched/game/colonist_at_cell.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char colony_cell_colonist(col_i16 i, col_i16 j);
col_i16 colonist_job(col_i16 c);
col_i16 colonist_specialty_get(col_i16 k);

col_i16 colonist_at_cell(col_i16 x, col_i16 y, col_i16 *spec_out)
{
    register col_i16 r = -1;
    register col_i16 colonist;

    colonist = colony_cell_colonist(x, y);
    if (colonist < 0)
        goto done;
    r = colonist_job(colonist);
    *spec_out = colonist_specialty_get(colonist);
done:
    return r;
}
