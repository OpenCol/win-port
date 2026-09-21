/* font_text_extent_1040_44a9 -- shipped at 1040:44a9.
 *
 * Ported by tools/port.py from matched/game/font_text_extent_1040_44a9.c.
 *
 * The file's own FontRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 text_extent(col_u16 hfont, const char *s);

col_i16 font_text_extent_1040_44a9(FontRec *f, const char *s)
{
    return text_extent(f->hfont, s);
}
