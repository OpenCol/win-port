/* MSWriteFile_1068_dc9a -- shipped at 1068:dc9a.
 *
 * Ported by tools/port.py from matched/gfx/MSWriteFile_1068_dc9a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSWriteFile(void *f, void *buf, col_i32 n);

col_i16 MSWriteFile_1068_dc9a(void *f, void *buf, col_i32 n)
{
    return MSWriteFile(f, buf, n);
}
