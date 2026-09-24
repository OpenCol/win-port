/* MSSetFocus_of_1058_c508 -- shipped at 1058:c508.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1058_c508.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *win; } GameWinBase;
extern void MSSetFocus(void *w);

void MSSetFocus_of_1058_c508(GameWinBase *o)
{
    MSSetFocus(o->win);
}
