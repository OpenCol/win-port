/* win_minimize_of -- shipped at 1048:d920.
 *
 * Ported by tools/port.py from matched/game/win_minimize_of.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void win_minimize(void *w);

void win_minimize_of(Obj *o)
{
    win_minimize((*((void * *)((unsigned char *)(o) + 0x6))));
}
