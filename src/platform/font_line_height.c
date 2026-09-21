/* font_line_height -- shipped at 1078:140e.
 *
 * Ported by tools/port.py from matched/platform/font_line_height.c.
 *
 * The file's own TEXTMETRIC, FontRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetTextMetrics(col_u16 hdc, TEXTMETRIC *tm);;
extern col_i16 pascal ReleaseDC(col_u16 hwnd, col_u16 hdc);;
extern col_u16 pascal GetDC(col_u16 hwnd);;
extern col_u16 pascal SelectObject(col_u16 hdc, col_u16 hobj);;
#endif

extern FontRec * MSHLock(col_u16 h);
extern void MSHUnlock(col_u16 h);





col_i16 font_line_height(col_u16 h)
{
    FontRec *f;
    col_u16 old;
    TEXTMETRIC tm;
    col_u16 dc;

    f = MSHLock(h);
    dc = GetDC(0);
    old = SelectObject(dc, f->hfont);
    GetTextMetrics(dc, &tm);
    SelectObject(dc, old);
    ReleaseDC(0, dc);
    MSHUnlock(h);
    return tm.tmHeight + tm.tmExternalLeading;
}
