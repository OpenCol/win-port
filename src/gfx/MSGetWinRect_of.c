/* MSGetWinRect_of -- shipped at 1068:df03.
 *
 * Ported by tools/port.py from matched/gfx/MSGetWinRect_of.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSGetWinRect(void *w, void *p);

void MSGetWinRect_of(Obj *o, void *p)
{
    MSGetWinRect((*((void * *)((unsigned char *)(o) + 0x6))), p);
}
