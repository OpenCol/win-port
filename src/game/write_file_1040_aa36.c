/* write_file_1040_aa36 -- shipped at 1040:aa36.
 *
 * Ported by tools/port.py from matched/game/write_file_1040_aa36.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSWriteFile(void *f, void *buf, col_i32 n);

col_i16 write_file_1040_aa36(void *f, void *buf, col_i32 n)
{
    return MSWriteFile(f, buf, n);
}
