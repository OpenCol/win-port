/* menu_enable_pos -- shipped at 1070:d658.
 *
 * Ported by tools/port.py from matched/platform/menu_enable_pos.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal EnableMenuItem(col_u16 hmenu, col_u16 pos, col_u16 flags);
extern col_u16 pascal GetSubMenu(col_u16 hmenu, col_i16 pos);
#endif

void menu_enable_pos(col_u16 hmenu, col_i16 pos, col_i16 item, char enable)
{
    col_u16 sub;

    if (item == 0) {
        if (enable == 0)
            EnableMenuItem(hmenu, pos - 1, 0x403);
        else
            EnableMenuItem(hmenu, pos - 1, 0x400);
    } else {
        sub = GetSubMenu(hmenu, pos - 1);
        if (enable == 0)
            EnableMenuItem(sub, item - 1, 0x403);
        else
            EnableMenuItem(sub, item - 1, 0x400);
    }
}
