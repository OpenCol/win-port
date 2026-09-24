/* menu_insert_1048_e260 -- shipped at 1048:e260.
 *
 * Ported by tools/port.py from matched/game/menu_insert_1048_e260.c.
 *
 * The file's own MenuRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void menu_insert_1070_d855(col_u16 hmenu, col_i16 pos, col_i16 item, const char *text, col_u16 id);

void menu_insert_1048_e260(MenuRec *m, col_i16 pos, col_i16 item, const char *text, col_u16 id)
{
    menu_insert_1070_d855(m->hmenu, pos, item, text, id);
}
