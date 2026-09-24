/* write_file_1050_919a -- shipped at 1050:919a.
 *
 * Ported by tools/port.py from matched/game/write_file_1050_919a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSWriteFile(col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i32 e);

col_i16 write_file_1050_919a(col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i32 e)
{
    return MSWriteFile(a, b, c, d, e);
}
