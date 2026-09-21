/* MSModifyMenu -- shipped at 1070:da9f.
 *
 * Ported by tools/port.py from matched/platform/MSModifyMenu.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal ModifyMenu(col_u16 hmenu, col_u16 item, col_u16 flags, col_u16 id, char *s);
#endif

void MSModifyMenu(col_u16 hmenu, col_i32 id, char *text)
{
    if (hmenu != 0)
        ModifyMenu(hmenu, (col_u16)id, 0, (col_u16)id, text);
}
