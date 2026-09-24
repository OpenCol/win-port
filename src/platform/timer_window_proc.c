/* timer_window_proc -- shipped at 1078:1968.
 *
 * Ported by tools/port.py from matched/platform/timer_window_proc.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void timer_tick_slot(void *o, col_u16 id);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal DefWindowProc(col_u16 hwnd, col_u16 msg, col_u16 wparam, col_i32 lparam);
extern col_i32 pascal GetWindowLong(col_u16 hwnd, col_i16 index);
#endif

#include "w16const.h"


/* same object: 4-byte `push cs; call ` (rule 47) */

col_i32 pascal timer_window_proc(col_u16 hwnd, col_u16 msg, col_u16 wparam, col_i32 lparam)
{
    void *o = (void *)GetWindowLong(hwnd, 0);

    if (o == 0)
        return DefWindowProc(hwnd, msg, wparam, lparam);
    switch (msg) {
    case WM_TIMER:
        timer_tick_slot(o, wparam);
        break;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
}
