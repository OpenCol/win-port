/* set_cursor -- shipped at 1078:28ce.
 *
 * Ported by tools/port.py from matched/platform/set_cursor.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal SetCursor(col_u16 h);
#endif

void set_cursor(col_u16 h)
{
    SetCursor(h);
}
