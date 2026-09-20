/* flag_86_high_bit -- shipped at 1050:0a88.
 *
 * Ported by tools/port.py from matched/game/flag_86_high_bit.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 flag_86_high_bit(Rec *r)
{
    return (r->flags & 0x80) != 0;
}
