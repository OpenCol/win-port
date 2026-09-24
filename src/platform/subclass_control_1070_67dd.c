/* subclass_control_1070_67dd -- shipped at 1070:67dd.
 *
 * Ported by tools/port.py from matched/platform/subclass_control_1070_67dd.c.
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

extern col_i32 object_message_handler(void);

void subclass_control_1070_67dd(col_i32 v, Ctl *c)
{
    SetWindowLong(c->hwnd, 4, v);
    SetWindowLong(c->hwnd, -4, (col_i32)(void *)object_message_handler);
}
