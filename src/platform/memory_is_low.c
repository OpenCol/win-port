/* memory_is_low -- shipped at 1070:d22d.
 *
 * Ported by tools/port.py from matched/platform/memory_is_low.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal GetFreeSystemResources(col_u16 which);;
extern col_u32 pascal GetFreeSpace(col_u16 flags);;
#endif

col_i16 memory_is_low(col_i16 kb)
{
    register col_i16 low = 0;
    col_u32 need = (col_u32)kb << 10;

    if (GetFreeSpace(0) < need)
        low = 1;
    if (GetFreeSystemResources(1) < 15)
        low = 1;
    if (GetFreeSystemResources(2) < 15)
        low = 1;
    if (GetFreeSystemResources(0) < 15)
        low = 1;
    return low;
}
