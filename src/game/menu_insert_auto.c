/* menu_insert_auto -- shipped at 1048:e238.
 *
 * Ported by tools/port.py from matched/game/menu_insert_auto.c.
 *
 * The file's own MenuRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void menu_insert_auto_id(col_u16 hmenu, col_i16 pos, col_i16 item, const char *text);

void menu_insert_auto(MenuRec *m, col_i16 pos, col_i16 item, const char *text)
{
    menu_insert_auto_id(m->hmenu, pos, item, text);
}
