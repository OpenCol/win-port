/* seed_nonzero_random -- shipped at 1068:0563.
 *
 * Ported by tools/port.py from matched/gfx/seed_nonzero_random.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 rand_rtl(void);

void seed_nonzero_random(Obj *o)
{
    o->seed = 0;
    while (o->seed == 0)
        o->seed = rand_rtl() & 0x7fff;
}
