/* font_metrics -- shipped at 1078:139b.
 *
 * Ported by tools/port.py from matched/platform/font_metrics.c.
 *
 * The file's own FontRec, TEXTMETRIC dropped for the merged record in col/records.h.
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



col_i16 font_metrics(col_u16 hfont)
{
    FontRec *f;
    col_u16 hdc;
    col_u16 old;
    TEXTMETRIC tm;

    f = MSHLock(hfont);
    hdc = GetDC(0);
    old = SelectObject(hdc, f->hfont);
    GetTextMetrics(hdc, &tm);
    SelectObject(hdc, old);
    ReleaseDC(0, hdc);
    MSHUnlock(hfont);
    return tm.tmHeight;
}
