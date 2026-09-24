/* MSSetFocus_of_1008_d136 -- shipped at 1008:d136.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1008_d136.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSSetFocus(void *w);

void MSSetFocus_of_1008_d136(GameWinBase *o)
{
    MSSetFocus(o->win);
}
