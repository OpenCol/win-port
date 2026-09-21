/* menu_enable_item_1070_da13 -- shipped at 1070:da13.
 *
 * Ported by tools/port.py from matched/platform/menu_enable_item_1070_da13.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal EnableMenuItem(col_u16 hmenu, col_u16 id, col_u16 flags);
#endif

void menu_enable_item_1070_da13(col_u16 hmenu, col_u32 id, char enable)
{
    if (hmenu != 0) {
        if (enable == 0)
            EnableMenuItem(hmenu, (col_u16)id, 3);
        else
            EnableMenuItem(hmenu, (col_u16)id, 0);
    }
}
