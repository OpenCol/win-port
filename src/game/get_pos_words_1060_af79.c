/* get_pos_words_1060_af79 -- shipped at 1060:af79.
 *
 * Ported by tools/port.py from matched/game/get_pos_words_1060_af79.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: a -> x_72, b -> y_74
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void get_pos_words_1060_af79(Rec *r, col_i16 *pa, col_i16 *pb)
{
    *pa = r->x_72;
    *pb = r->y_74;
}
