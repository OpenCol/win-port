/* MSCheckMenuItem -- shipped at 1070:d6d5.
 *
 * Ported by tools/port.py from matched/platform/MSCheckMenuItem.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal CheckMenuItem(col_u16 hmenu, col_u16 item, col_u16 flags);
extern col_u16 pascal GetSubMenu(col_u16 hmenu, col_i16 pos);
#endif

void MSCheckMenuItem(col_u16 hmenu, col_i16 pos, col_i16 item, char checked)
{
    col_u16 sub;

    if (hmenu != 0) {
        sub = GetSubMenu(hmenu, pos - 1);
        if (checked == 0)
            CheckMenuItem(sub, item - 1, 0x400);
        else
            CheckMenuItem(sub, item - 1, 0x408);
    }
}
