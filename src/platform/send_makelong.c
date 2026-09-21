/* send_makelong -- shipped at 1070:5afb.
 *
 * Ported by tools/port.py from matched/platform/send_makelong.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SendMessage(col_i16 h, col_i16 msg, col_i16 w, col_i32 l);;
#endif

void send_makelong(col_i16 h, col_i16 lo, col_i16 hi)
{
    SendMessage(h, 0x401, 0, (col_u32)(col_u16)lo | ((col_u32)(col_u16)hi << 16));
}
