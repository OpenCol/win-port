/* cursor_load -- shipped at 1008:d689.
 *
 * Ported by tools/port.py from matched/game/cursor_load.c.
 *
 * The file's own CursorClass dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_u16 MSLoadCursor(col_i16 id);

void cursor_load(CursorClass *c, col_i16 id)
{
    c->hcursor = MSLoadCursor(id);
    c->id = id;
}
