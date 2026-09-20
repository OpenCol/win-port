/* popup_set_width -- shipped at 1050:0cee.
 *
 * Ported by tools/port.py from matched/game/popup_set_width.c.
 *
 * The file's own Popup dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void popup_set_width(Popup *p, col_i16 v)
{
    p->width = v;
}
