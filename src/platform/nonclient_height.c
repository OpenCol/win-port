/* nonclient_height -- shipped at 1078:24f5.
 *
 * Ported by tools/port.py from matched/platform/nonclient_height.c.
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 nonclient_width(MSWin *w);
col_i16 rect_height_1078_3418(RECT *r);
col_i16 rect_width_1078_33f9(RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetClientRect(col_u16 hwnd, RECT *r);
extern col_i16 pascal GetWindowRect(col_u16 hwnd, RECT *r);
#endif

col_i16 nonclient_height(MSWin *w)
{
    col_i16 outer;
    col_i16 inner;
    RECT r;

    if (w == 0)
        return 0;
    GetWindowRect(w->hwnd, &r);
    outer = rect_height_1078_3418(&r);
    GetClientRect(w->hwnd, &r);
    inner = rect_height_1078_3418(&r);
    return outer - inner;
}
