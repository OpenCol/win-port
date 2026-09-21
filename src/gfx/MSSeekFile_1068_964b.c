/* MSSeekFile_1068_964b -- shipped at 1068:964b.
 *
 * Ported by tools/port.py from matched/gfx/MSSeekFile_1068_964b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 MSSeekFile(void *f, col_i32 pos);

col_i32 MSSeekFile_1068_964b(void *f, col_i32 pos)
{
    return MSSeekFile(f, pos);
}
