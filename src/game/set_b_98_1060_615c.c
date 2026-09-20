/* set_b_98_1060_615c -- shipped at 1060:615c.
 *
 * Ported by tools/port.py from matched/game/set_b_98_1060_615c.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_98
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_b_98_1060_615c(Rec *r, char v)
{
    r->v_98 = v;
}
