/* get_size_words_1070_9c10 -- shipped at 1070:9c10.
 *
 * Ported by tools/port.py from matched/platform/get_size_words_1070_9c10.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: a -> x, b -> y
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void get_size_words_1070_9c10(Rec *r, col_i16 *pa, col_i16 *pb)
{
    *pa = r->x;
    *pb = r->y;
}
