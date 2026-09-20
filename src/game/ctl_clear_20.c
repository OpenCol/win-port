/* ctl_clear_20 -- shipped at 1050:87ba.
 *
 * Ported by tools/port.py from matched/game/ctl_clear_20.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void ctl_clear_20(Ctl *c)
{
    c->w20 = 0;
}
