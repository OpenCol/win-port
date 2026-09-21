/* window_of -- shipped at 1060:ab6e.
 *
 * Ported by tools/port.py from matched/game/window_of.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * window_of(Obj *o)
{
    return (*((void * *)((unsigned char *)(o) + 0x6)));
}
