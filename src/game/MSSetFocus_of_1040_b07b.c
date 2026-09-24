/* MSSetFocus_of_1040_b07b -- shipped at 1040:b07b.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1040_b07b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSSetFocus(void *w);

void MSSetFocus_of_1040_b07b(GameWinBase *o)
{
    MSSetFocus(o->win);
}
