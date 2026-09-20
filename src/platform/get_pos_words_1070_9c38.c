/* get_pos_words_1070_9c38 -- shipped at 1070:9c38.
 *
 * Ported by tools/port.py from matched/platform/get_pos_words_1070_9c38.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: a -> x_72, b -> y_74
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void get_pos_words_1070_9c38(Rec *r, col_i16 *pa, col_i16 *pb)
{
    *pa = r->x_72;
    *pb = r->y_74;
}
