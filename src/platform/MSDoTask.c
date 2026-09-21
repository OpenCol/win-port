/* MSDoTask -- shipped at 1078:115b.
 *
 * Ported by tools/port.py from matched/platform/MSDoTask.c.
 *
 * The file's own MSG dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal PeekMessage(MSG *m, col_u16 hwnd, col_u16 lo, col_u16 hi, col_u16 remove);;
extern col_i16 pascal TranslateMessage(MSG *m);;
extern col_i32 pascal DispatchMessage(MSG *m);;
#endif

/* 18 bytes */




col_i16 MSDoTask(void)
{
    MSG m;

    if (PeekMessage(&m, 0, 0, 0, 1)) {
        TranslateMessage(&m);
        DispatchMessage(&m);
        return 1;
    } else {
        return 0;
    }
}
