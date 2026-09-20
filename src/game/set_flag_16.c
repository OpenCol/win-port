/* set_flag_16 -- shipped at 1050:8b46.
 *
 * Ported by tools/port.py from matched/game/set_flag_16.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_flag_16(Ctl *c)
{
    c->f16 = 1;
}
