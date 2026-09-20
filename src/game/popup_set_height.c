/* popup_set_height -- shipped at 1050:0d07.
 *
 * Ported by tools/port.py from matched/game/popup_set_height.c.
 *
 * The file's own Popup dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void popup_set_height(Popup *p, col_i16 v)
{
    p->height = v;
}
