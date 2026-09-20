/* set_b_98_1048_deb3 -- shipped at 1048:deb3.
 *
 * Ported by tools/port.py from matched/game/set_b_98_1048_deb3.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_98
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_b_98_1048_deb3(Rec *r, char v)
{
    r->v_98 = v;
}
