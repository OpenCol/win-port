/* set_long_0e -- shipped at 1078:1f27.
 *
 * Ported by tools/port.py from matched/platform/set_long_0e.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_long_0e(Rec *r, col_i32 v)
{
    (*((col_i32 *)((unsigned char *)(r) + 0xe))) = v;
}
