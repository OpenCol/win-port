/* clear_colonist_cell -- shipped at 1018:62db.
 *
 * Ported by tools/port.py from matched/game/clear_colonist_cell.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 find_colony_grid_cell(col_i16 id, void *a, void *b);
void colony_set_cell(col_i16 a, col_i16 b, col_i16 v);

/* same object: 4-byte `push cs; call ` (rule 47) */

void clear_colonist_cell(col_i16 id)
{
    col_i16 a;
    col_i16 b;

    if (find_colony_grid_cell(id, &a, &b))
        colony_set_cell(a, b, -1);
}
