/* text_extent -- shipped at 1078:1484.
 *
 * Ported by tools/port.py from matched/platform/text_extent.c.
 *
 * STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own FontRec, Size dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetTextExtentPoint(col_u16 hdc, const char *s, col_i16 n, Size *sz);
extern col_i16 pascal ReleaseDC(col_u16 hwnd, col_u16 hdc);
extern col_u16 pascal GetDC(col_u16 hwnd);
extern col_u16 pascal SelectObject(col_u16 hdc, col_u16 hobj);
#endif

extern FontRec * MSHLock(col_i16 h);
extern void MSHUnlock(col_i16 h);



col_i16 text_extent(col_u16 hfont, const char *s)
{
    FontRec *f;
    col_u16 hdc;
    col_u16 old;
    col_i16 w;
    Size sz;

    f = MSHLock(hfont);
    hdc = GetDC(0);
    old = SelectObject(hdc, f->hfont);
    GetTextExtentPoint(hdc, s, strlen(s), &sz);
    w = sz.cx;
    SelectObject(hdc, old);
    ReleaseDC(0, hdc);
    MSHUnlock(hfont);
    return w;
}
