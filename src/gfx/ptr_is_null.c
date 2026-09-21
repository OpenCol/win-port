/* ptr_is_null -- shipped at 1068:9dbb.
 *
 * Ported by tools/port.py from matched/gfx/ptr_is_null.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 ptr_is_null(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x0))) == 0;
}
