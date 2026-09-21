/* MSPositionFile_1068_9705 -- shipped at 1068:9705.
 *
 * Ported by tools/port.py from matched/gfx/MSPositionFile_1068_9705.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 MSPositionFile(void *f, col_i32 pos);

col_i32 MSPositionFile_1068_9705(void *f, col_i32 pos)
{
    return MSPositionFile(f, pos);
}
