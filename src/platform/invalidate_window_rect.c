/* invalidate_window_rect -- shipped at 1078:21f2.
 *
 * Ported by tools/port.py from matched/platform/invalidate_window_rect.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal InvalidateRect(col_u16 hwnd, void *rect, col_i16 erase);
#endif

void invalidate_window_rect(MSWin *w, void *rect)
{
    if (w != 0)
        InvalidateRect(w->hwnd, rect, 0);
}
