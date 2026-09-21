/* kill_timer -- shipped at 1078:194f.
 *
 * Ported by tools/port.py from matched/platform/kill_timer.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal KillTimer(col_u16 hwnd, col_i16 id);;
#endif

void kill_timer(col_u16 hwnd, col_i16 id)
{
    KillTimer(hwnd, id);
}
