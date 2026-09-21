/* get_window_text -- shipped at 1070:5a90.
 *
 * Ported by tools/port.py from matched/platform/get_window_text.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SendMessage(col_i16 h, col_i16 msg, col_i16 w, col_i32 l);;
#endif

void get_window_text(col_i16 h, col_i32 buf)
{
    SendMessage(h, 0xd, SendMessage(h, 0xe, 0, 0L) + 1, buf);
}
