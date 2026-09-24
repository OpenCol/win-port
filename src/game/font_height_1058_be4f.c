/* font_height_1058_be4f -- shipped at 1058:be4f.
 *
 * Ported by tools/port.py from matched/game/font_height_1058_be4f.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: v -> v_2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 font_height_1058_be4f(Rec *r)
{
    return r->v_2;
}
