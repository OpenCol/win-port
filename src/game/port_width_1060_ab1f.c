/* port_width_1060_ab1f -- shipped at 1060:ab1f.
 *
 * Ported by tools/port.py from matched/game/port_width_1060_ab1f.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 port_width_1060_ab1f(Rec *r)
{
    return r->v_2;
}
