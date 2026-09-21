/* MSDeleteMenuItem_1008_d791 -- shipped at 1008:d791.
 *
 * Ported by tools/port.py from matched/game/MSDeleteMenuItem_1008_d791.c.
 *
 * The file's own MenuRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSDeleteMenuItem(col_i16 menu, col_i16 a, col_i16 b);

void MSDeleteMenuItem_1008_d791(MenuRec *m, col_i16 a, col_i16 b)
{
    MSDeleteMenuItem(m->hmenu, a, b);
}
