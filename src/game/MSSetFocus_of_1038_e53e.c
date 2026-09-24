/* MSSetFocus_of_1038_e53e -- shipped at 1038:e53e.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1038_e53e.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSSetFocus(void *w);

void MSSetFocus_of_1038_e53e(GameWinBase *o)
{
    MSSetFocus(o->win);
}
