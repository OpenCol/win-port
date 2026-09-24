/* port_width_1008_d707 -- shipped at 1008:d707.
 *
 * Ported by tools/port.py from matched/game/port_width_1008_d707.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 port_width_1008_d707(Rec *r)
{
    return r->v_2;
}
