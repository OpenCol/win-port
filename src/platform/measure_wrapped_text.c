/* measure_wrapped_text -- shipped at 1078:173f.
 *
 * Ported by tools/port.py from matched/platform/measure_wrapped_text.c.
 *
 * STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own FontRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DeleteDC(col_u16 hdc);;
extern col_i16 pascal DrawText(col_u16 hdc, const char *s, col_i16 n, RECT *r, col_u16 fmt);;
extern col_i16 pascal ReleaseDC(col_u16 hwnd, col_u16 hdc);;
extern col_u16 pascal CreateCompatibleDC(col_u16 hdc);;
extern col_u16 pascal GetDC(col_u16 hwnd);;
extern col_u16 pascal SelectObject(col_u16 hdc, col_u16 hobj);;
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);;
#endif

extern FontRec * MSHLock_1068_93af(col_u16 h);
extern void MSHUnlock_1068_93cc(col_u16 h);









RECT * measure_wrapped_text(RECT *out, col_u16 font, char *s, col_i16 width)
{
    col_u16 dc0;
    col_u16 old;
    RECT rc;
    FontRec *f;
    col_u16 dc;

    f = MSHLock_1068_93af(font);
    dc0 = GetDC(0);
    dc = CreateCompatibleDC(dc0);
    ReleaseDC(0, dc0);
    old = SelectObject(dc, f->hfont);
    SetRect(&rc, 0, 0, width, 0);
    DrawText(dc, s, strlen(s), &rc, 0x410);
    SelectObject(dc, old);
    DeleteDC(dc);
    MSHUnlock_1068_93cc(font);
    out->left = rc.left;
    out->top = rc.top;
    out->right = rc.right;
    out->bottom = rc.bottom;
    return out;
}
