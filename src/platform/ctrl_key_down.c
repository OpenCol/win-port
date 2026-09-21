/* ctrl_key_down -- shipped at 1070:9b58.
 *
 * Ported by tools/port.py from matched/platform/ctrl_key_down.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetKeyState(col_i16 vk);;
#endif

col_i16 ctrl_key_down(void)
{
    return (GetKeyState(0x11) & 0x8000) != 0;
}
