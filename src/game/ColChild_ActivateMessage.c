/* ColChild_ActivateMessage -- shipped at 1048:126c.
 *
 * Ported by tools/port.py from matched/game/ColChild_ActivateMessage.c.
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void ColChild_ActivateMessage(ColChild *c, char kind, col_i32 arg)
{
    c->active = 1;
    c->kind = kind;
}
