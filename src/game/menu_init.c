/* menu_init -- shipped at 1048:e111.
 *
 * Ported by tools/port.py from matched/game/menu_init.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_u16 MSInitMenu(col_i16 id);

void menu_init(col_u16 *slot, col_i16 id)
{
    *slot = MSInitMenu(id);
}
