/* set_b_98_1058_b992 -- shipped at 1058:b992.
 *
 * Ported by tools/port.py from matched/game/set_b_98_1058_b992.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_98
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_b_98_1058_b992(Rec *r, char v)
{
    r->v_98 = v;
}
