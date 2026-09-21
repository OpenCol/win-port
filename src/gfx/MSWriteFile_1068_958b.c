/* MSWriteFile_1068_958b -- shipped at 1068:958b.
 *
 * Ported by tools/port.py from matched/gfx/MSWriteFile_1068_958b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSWriteFile(void *f, void *buf, col_i32 n);

col_i16 MSWriteFile_1068_958b(void *f, void *buf, col_i32 n)
{
    return MSWriteFile(f, buf, n);
}
