/* draw_ctl_border -- shipped at 1070:623d.
 *
 * Ported by tools/port.py from matched/platform/draw_ctl_border.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DeleteObject(col_u16 obj);
extern col_i16 pascal GetClientRect(col_u16 hwnd, RECT *r);
extern col_i16 pascal LineTo(col_u16 hdc, col_i16 x, col_i16 y);
extern col_i32 pascal MoveTo(col_u16 hdc, col_i16 x, col_i16 y);
extern col_u16 pascal CreatePen(col_i16 style, col_i16 width, col_i32 colour);
extern col_u16 pascal SelectObject(col_u16 hdc, col_u16 obj);
#endif

#include "w16const.h"








void draw_ctl_border(col_u16 hwnd, register col_u16 hdc, col_u32 style)
{
    col_u16 white;
    col_u16 grey;
    RECT r;
    col_u16 black;

    black = CreatePen(PS_SOLID, 1, 0x000000L);
    white = CreatePen(PS_SOLID, 1, 0xffffffL);
    grey = CreatePen(PS_SOLID, 1, 0x808080L);
    GetClientRect(hwnd, &r);
    if (style & 1) {
        SelectObject(hdc, black);
        MoveTo(hdc, 0, 0);
        LineTo(hdc, r.right, 0);
        LineTo(hdc, r.right, r.bottom);
        LineTo(hdc, 0, r.bottom);
        LineTo(hdc, 0, 0);
    } else if (style & 2) {
        SelectObject(hdc, black);
        MoveTo(hdc, 0, 0);
        LineTo(hdc, r.right, 0);
        LineTo(hdc, r.right, r.bottom);
        LineTo(hdc, 0, r.bottom);
        LineTo(hdc, 0, 0);
        SelectObject(hdc, grey);
        MoveTo(hdc, r.right - 1, 1);
        LineTo(hdc, 1, 1);
        LineTo(hdc, 1, r.bottom - 1);
        SelectObject(hdc, white);
        LineTo(hdc, r.right - 1, r.bottom - 1);
        LineTo(hdc, r.right - 1, 1);
    } else if (style & 4) {
        SelectObject(hdc, black);
        MoveTo(hdc, 0, 0);
        LineTo(hdc, r.right, 0);
        LineTo(hdc, r.right, r.bottom);
        LineTo(hdc, 0, r.bottom);
        LineTo(hdc, 0, 0);
        SelectObject(hdc, white);
        MoveTo(hdc, r.right - 1, 1);
        LineTo(hdc, 1, 1);
        LineTo(hdc, 1, r.bottom - 1);
        SelectObject(hdc, grey);
        LineTo(hdc, r.right - 1, r.bottom - 1);
        LineTo(hdc, r.right - 1, 1);
    }
    DeleteObject(black);
    DeleteObject(white);
    DeleteObject(grey);
}
