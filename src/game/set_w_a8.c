/* set_w_a8 -- shipped at 1050:102c.
 *
 * Ported by tools/port.py from matched/game/set_w_a8.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_w_a8(Rec *r, col_i16 v)
{
    (*((col_i16 *)((unsigned char *)(r) + 0xa8))) = v;
}
