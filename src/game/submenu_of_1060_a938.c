/* submenu_of_1060_a938 -- shipped at 1060:a938.
 *
 * Ported by tools/port.py from matched/game/submenu_of_1060_a938.c.
 *
 * The file's own MenuObj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_u16 MSGetSubMenu(col_u16 hmenu, col_i16 pos);

col_u16 submenu_of_1060_a938(MenuObj *o, col_i16 pos)
{
    return MSGetSubMenu(o->hmenu, pos);
}
