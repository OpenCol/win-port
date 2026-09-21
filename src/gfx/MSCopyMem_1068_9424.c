/* MSCopyMem_1068_9424 -- shipped at 1068:9424.
 *
 * Ported by tools/port.py from matched/gfx/MSCopyMem_1068_9424.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSCopyMem(void *src, void *dst, col_i32 n);

void MSCopyMem_1068_9424(void *src, void *dst, col_i32 n)
{
    MSCopyMem(src, dst, n);
}
