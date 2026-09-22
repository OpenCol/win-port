/* save_read_field_1068_dc06 -- shipped at 1068:dc06.
 *
 * Ported by tools/port.py from matched/gfx/save_read_field_1068_dc06.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 MSReadFile(void *f, void *buf, col_i32 n);

col_i16 save_read_field_1068_dc06(void *f, void *buf, col_i32 n)
{
    return MSReadFile(f, buf, n);
}
