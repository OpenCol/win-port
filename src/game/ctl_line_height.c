/* ctl_line_height -- shipped at 1050:834b.
 *
 * Ported by tools/port.py from matched/game/ctl_line_height.c.
 *
 * The file's own FontRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 font_line_height(col_u16 h);

col_i16 ctl_line_height(FontRec *f)
{
    return font_line_height(f->hfont);
}
