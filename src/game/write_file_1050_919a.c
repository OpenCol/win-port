/* write_file_1050_919a -- shipped at 1050:919a.
 *
 * Ported by tools/port.py from matched/game/write_file_1050_919a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSWriteFile(void *f, void *buf, col_i32 n);

col_i16 write_file_1050_919a(void *f, void *buf, col_i32 n)
{
    return MSWriteFile(f, buf, n);
}
