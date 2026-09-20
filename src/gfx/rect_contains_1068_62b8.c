/* rect_contains_1068_62b8 -- shipped at 1068:62b8.
 *
 * Ported by tools/port.py from matched/gfx/rect_contains_1068_62b8.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: bottom -> fe, top -> v_a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char rect_contains_1068_62b8(Rec *r, col_i16 x, col_i16 y)
{
    if (r->left > x || r->right <= x || r->v_a > y || r->fe <= y)
        return 0;
    else
        return 1;
}
