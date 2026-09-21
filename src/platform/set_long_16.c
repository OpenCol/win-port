/* set_long_16 -- shipped at 1078:1f5b.
 *
 * Ported by tools/port.py from matched/platform/set_long_16.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_long_16(Rec *r, col_i32 v)
{
    (*((col_i32 *)((unsigned char *)(r) + 0x16))) = v;
}
