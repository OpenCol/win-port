/* panel_clear_4 -- shipped at 1008:357e.
 *
 * Ported by tools/port.py from matched/game/panel_clear_4.c.
 *
 * The file's own Panel dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void panel_clear_4(Panel *p, col_i16 n)
{
    p->w4 = 0;
}
