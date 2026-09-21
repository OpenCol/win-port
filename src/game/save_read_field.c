/* save_read_field -- shipped at 1008:d834.
 *
 * Ported by tools/port.py from matched/game/save_read_field.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSReadFile(void *f, void *buf, col_i32 n);

col_i16 save_read_field(void *f, void *buf, col_i32 n)
{
    return MSReadFile(f, buf, n);
}
