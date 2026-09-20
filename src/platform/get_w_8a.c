/* get_w_8a -- shipped at 1070:9fcc.
 *
 * Ported by tools/port.py from matched/platform/get_w_8a.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 get_w_8a(Rec *r)
{
    return (*((col_i16 *)((unsigned char *)(r) + 0x8a)));
}
