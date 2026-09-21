/* MSSetFocus_of_1010_eb8f -- shipped at 1010:eb8f.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1010_eb8f.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSSetFocus(void *w);

void MSSetFocus_of_1010_eb8f(Obj *o)
{
    MSSetFocus((*((void * *)((unsigned char *)(o) + 0x6))));
}
