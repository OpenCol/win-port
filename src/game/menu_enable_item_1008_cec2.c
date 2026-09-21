/* menu_enable_item_1008_cec2 -- shipped at 1008:cec2.
 *
 * Ported by tools/port.py from matched/game/menu_enable_item_1008_cec2.c.
 *
 * The file's own MenuRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void menu_enable_item_1070_da13(col_u16 hmenu, col_u32 id, char enable);

void menu_enable_item_1008_cec2(MenuRec *m, col_u32 id, char enable)
{
    menu_enable_item_1070_da13(m->hmenu, id, enable);
}
