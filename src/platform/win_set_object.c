/* win_set_object -- shipped at 1070:6811.
 *
 * Ported by tools/port.py from matched/platform/win_set_object.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: hwnd -> hdc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SetWindowLong(col_u16 hwnd, col_i16 index, col_i32 value);
#endif

void win_set_object(void *o, Win *w)
{
    SetWindowLong(w->hdc, 4, (col_i32)o);
}
