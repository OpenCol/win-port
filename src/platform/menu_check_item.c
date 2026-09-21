/* menu_check_item -- shipped at 1070:da48.
 *
 * Ported by tools/port.py from matched/platform/menu_check_item.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal CheckMenuItem(col_u16 hmenu, col_u16 id, col_u16 flags);
#endif

void menu_check_item(col_u16 hmenu, col_u32 id, char check)
{
    if (hmenu != 0) {
        if (check == 0)
            CheckMenuItem(hmenu, (col_u16)id, 0);
        else
            CheckMenuItem(hmenu, (col_u16)id, 8);
    }
}
