/* mouse_right_down_1078_2cfb -- shipped at 1078:2cfb.
 *
 * Ported by tools/port.py from matched/platform/mouse_right_down_1078_2cfb.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal GetAsyncKeyState(col_i16 vk);
#endif

col_i16 mouse_right_down_1078_2cfb(void)
{
    if ((unsigned char)(GetAsyncKeyState(2) >> 8) != 0)
        return 1;
    return 0;
}
