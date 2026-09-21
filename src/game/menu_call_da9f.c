/* menu_call_da9f -- shipped at 1048:e300.
 *
 * Ported by tools/port.py from matched/game/menu_call_da9f.c.
 *
 * The file's own MenuRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSModifyMenu(col_i16 hmenu, col_i32 a, col_i16 b, col_i16 c);

void menu_call_da9f(MenuRec *m, col_i32 a, col_i16 b, col_i16 c)
{
    MSModifyMenu(m->hmenu, a, b, c);
}
