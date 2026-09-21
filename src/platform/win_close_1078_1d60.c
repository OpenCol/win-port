/* win_close_1078_1d60 -- shipped at 1078:1d60.
 *
 * Ported by tools/port.py from matched/platform/win_close_1078_1d60.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u32 pascal SendMessage(col_u16 hwnd, col_u16 msg, col_u16 wparam, col_i32 lparam);
#endif

void win_close_1078_1d60(MSWin *w)
{
    if (w != 0)
        SendMessage(w->hwnd, 0x10, 0, 0L);
}
