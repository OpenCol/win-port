/* ColChild_CancelMessage -- shipped at 1048:10c6.
 *
 * Ported by tools/port.py from matched/game/ColChild_CancelMessage.c.
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: flag1 -> active, flag2 -> kind
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void ColChild_CancelMessage(ColChild *c, col_i16 a)
{
    c->active = 0;
    c->kind = 0;
    ((char *)((unsigned char *)(c) + 0x149))[0] = 0;
}
