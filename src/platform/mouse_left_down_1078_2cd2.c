/* mouse_left_down_1078_2cd2 -- shipped at 1078:2cd2.
 *
 * Ported by tools/port.py from matched/platform/mouse_left_down_1078_2cd2.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal GetAsyncKeyState(col_i16 vk);
#endif

col_i16 mouse_left_down_1078_2cd2(void)
{
    if ((unsigned char)(GetAsyncKeyState(1) >> 8) != 0)
        return 1;
    return 0;
}
