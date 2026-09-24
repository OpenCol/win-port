/* MSAllocateHandle_1068_93e9 -- shipped at 1068:93e9.
 *
 * Ported by tools/port.py from matched/gfx/MSAllocateHandle_1068_93e9.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_u16 MSAllocateHandle(col_u32 size);

col_i16 MSAllocateHandle_1068_93e9(col_i32 size)
{
    return MSAllocateHandle(size);
}
