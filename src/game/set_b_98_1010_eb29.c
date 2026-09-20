/* set_b_98_1010_eb29 -- shipped at 1010:eb29.
 *
 * Ported by tools/port.py from matched/game/set_b_98_1010_eb29.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_98
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_b_98_1010_eb29(Rec *r, char v)
{
    r->v_98 = v;
}
