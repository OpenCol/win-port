/* deref_long -- shipped at 1068:d5fe.
 *
 * Ported by tools/port.py from matched/gfx/deref_long.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i32 deref_long(col_i32 *p)
{
    return *p;
}
