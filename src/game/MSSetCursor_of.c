/* MSSetCursor_of -- shipped at 1038:e219.
 *
 * Ported by tools/port.py from matched/game/MSSetCursor_of.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSSetCursor(void *w, void *p);

void MSSetCursor_of(Obj *o, void *p)
{
    MSSetCursor((*((void * *)((unsigned char *)(o) + 0x6))), p);
}
