/* set_defaults -- shipped at 1008:378f.
 *
 * Ported by tools/port.py from matched/game/set_defaults.c.
 *
 * The file's own TurnCtx dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_defaults(TurnCtx *t)
{
    t->w4 = 100;
    t->w6 = 0;
    t->w2 = 0;
}
