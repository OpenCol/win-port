/* init_record -- shipped at 1040:6b19.
 *
 * Ported by tools/port.py from matched/game/init_record.c.
 *
 * The file's own Rec14 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void init_record(Rec14 *r, col_i16 a, col_i16 b, col_i32 c, col_i16 d)
{
    r->f1 = 0;
    r->f0 = a;
    r->f2 = b;
    r->f4 = c;
    r->f8 = 0;
    r->fa = r->fc = d;
}
