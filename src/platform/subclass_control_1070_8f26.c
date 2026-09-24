/* subclass_control_1070_8f26 -- shipped at 1070:8f26.
 *
 * Ported by tools/port.py from matched/platform/subclass_control_1070_8f26.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SetWindowLong(col_i16 h, col_i16 idx, col_i32 v);
#endif

extern col_i32 dialog_wndproc(void);

void subclass_control_1070_8f26(col_i32 v, Ctl *c)
{
    SetWindowLong(c->hwnd, 8, v);
    SetWindowLong(c->hwnd, -4, (col_i32)(void *)dialog_wndproc);
}
