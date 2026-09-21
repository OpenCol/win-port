/* MSDeleteMenuItem_1048_e2bb -- shipped at 1048:e2bb.
 *
 * Ported by tools/port.py from matched/game/MSDeleteMenuItem_1048_e2bb.c.
 *
 * The file's own MenuRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSDeleteMenuItem(col_i16 menu, col_i16 a, col_i16 b);

void MSDeleteMenuItem_1048_e2bb(MenuRec *m, col_i16 a, col_i16 b)
{
    MSDeleteMenuItem(m->hmenu, a, b);
}
