/* save_write_field -- shipped at 1008:d8c4.
 *
 * Ported by tools/port.py from matched/game/save_write_field.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSWriteFile(void *f, void *buf, col_i32 n);

col_i16 save_write_field(void *f, void *buf, col_i32 n)
{
    return MSWriteFile(f, buf, n);
}
