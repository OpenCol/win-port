/* dispose_wing_surface -- shipped at 1070:a64b.
 *
 * Ported by tools/port.py from matched/platform/dispose_wing_surface.c.
 *
 * The file's own WinGRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DeleteDC(col_u16 hdc);
extern col_i16 pascal DeleteObject(col_u16 obj);
extern col_u16 pascal SelectObject(col_u16 hdc, col_u16 obj);
#endif

extern void MSHUnlock(col_u16 h);
extern void MSDisposeHandle(col_u16 h);

void * dispose_wing_surface(WinGRec *r)
{
    register col_u16 h;

    if (r == 0)
        return 0;
    if ((*((col_u16 *)((unsigned char *)(r) + 0x2))) != 0) {
        SelectObject((*((col_u16 *)((unsigned char *)(r) + 0x2))), (*((col_u16 *)((unsigned char *)(r) + 0x8))));
        DeleteObject(r->hbitmap);
        DeleteDC((*((col_u16 *)((unsigned char *)(r) + 0x2))));
    }
    if ((*((col_u16 *)((unsigned char *)(r) + 0x4))) != 0)
        DeleteObject((*((col_u16 *)((unsigned char *)(r) + 0x4))));
    h = (*((col_u16 *)((unsigned char *)(r) + 0x0)));
    MSHUnlock(h);
    MSDisposeHandle(h);
    return 0;
}
