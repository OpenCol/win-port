/* set_font_from_handle -- shipped at 1070:5452.
 *
 * Ported by tools/port.py from matched/platform/set_font_from_handle.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SendMessage(col_i16 h, col_i16 msg, col_i16 w, col_i32 l);;
#endif

extern void * MSHLock(col_i16 h);
extern col_i32 MSHUnlock(col_i16 h);


void set_font_from_handle(col_i16 hwnd, col_i16 hmem)
{
    col_i16 *p;

    p = MSHLock(hmem);
    SendMessage(hwnd, 0x30, *p, 0L);
    MSHUnlock(hmem);
}
