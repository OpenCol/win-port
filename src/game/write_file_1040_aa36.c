/* write_file_1040_aa36 -- shipped at 1040:aa36.
 *
 * Ported by tools/port.py from matched/game/write_file_1040_aa36.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSWriteFile(col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i32 e);

col_i16 write_file_1040_aa36(col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i32 e)
{
    return MSWriteFile(a, b, c, d, e);
}
