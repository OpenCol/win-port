/* menu_enable_item_1048_e2dd -- shipped at 1048:e2dd.
 *
 * Ported by tools/port.py from matched/game/menu_enable_item_1048_e2dd.c.
 *
 * The file's own MenuRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void menu_enable_item_1070_da13(col_u16 hmenu, col_u32 id, char enable);

void menu_enable_item_1048_e2dd(MenuRec *m, col_u32 id, char enable)
{
    menu_enable_item_1070_da13(m->hmenu, id, enable);
}
