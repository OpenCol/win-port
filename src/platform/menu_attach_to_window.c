/* menu_attach_to_window -- shipped at 1078:1d88.
 *
 * Ported by tools/port.py from matched/platform/menu_attach_to_window.c.
 *
 * The file's own Menu dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DeleteObject(col_u16 hobj);
extern col_i16 pascal DestroyWindow(col_u16 hwnd);
extern col_i16 pascal SetMenu(col_u16 hwnd, col_u16 hmenu);
extern void pascal ShowWindow(col_u16 hwnd, col_i16 cmd);
#endif

extern void check_ms_window_stack(col_u16 hwnd);




extern void MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_u16 h);

void * menu_attach_to_window(Menu *m)
{
    col_u16 h;

    if (m != 0) {
        check_ms_window_stack(m->hwnd);
        if (m->shared == 0) {
            if (m->hwnd != 0) {
                ShowWindow(m->hwnd, 0);
                SetMenu(m->hwnd, 0);
                DestroyWindow(m->hwnd);
            }
            m->hwnd = 0;
        }
        if (m->hobj != 0)
            DeleteObject(m->hobj);
        h = m->handle;
        MSHUnlock(h);
        MSDisposeHandle(h);
    }
    return 0;
}
