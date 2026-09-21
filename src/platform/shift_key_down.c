/* shift_key_down -- shipped at 1070:9b7d.
 *
 * Ported by tools/port.py from matched/platform/shift_key_down.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetKeyState(col_i16 vk);;
#endif

col_i16 shift_key_down(void)
{
    return (GetKeyState(0x10) & 0x8000) != 0;
}
