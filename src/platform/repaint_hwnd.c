/* repaint_hwnd -- shipped at 1070:0ae5.
 *
 * Ported by tools/port.py from matched/platform/repaint_hwnd.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal GetClientRect(col_u16 hwnd, RECT *r);
extern void pascal InvalidateRect(col_u16 hwnd, RECT *r, col_i16 erase);
extern void pascal UpdateWindow(col_u16 hwnd);
#endif

void repaint_hwnd(col_u16 hwnd)
{
    RECT r;

    GetClientRect(hwnd, &r);
    InvalidateRect(hwnd, &r, 1);
    UpdateWindow(hwnd);
}
