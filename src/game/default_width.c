/* default_width -- shipped at 1050:0aac.
 *
 * Ported by tools/port.py from matched/game/default_width.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: kind -> v
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 default_width(Rec *r)
{
    col_i16 w;

    switch (r->v) {
    default:
        w = 0x20;
        break;
    }
    return w;
}
