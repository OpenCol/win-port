/* MSSetFocus_of_1050_954c -- shipped at 1050:954c.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1050_954c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSSetFocus(void *w);

void MSSetFocus_of_1050_954c(GameWinBase *o)
{
    MSSetFocus(o->win);
}
