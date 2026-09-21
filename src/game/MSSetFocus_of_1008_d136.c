/* MSSetFocus_of_1008_d136 -- shipped at 1008:d136.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1008_d136.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSSetFocus(void *w);

void MSSetFocus_of_1008_d136(Obj *o)
{
    MSSetFocus((*((void * *)((unsigned char *)(o) + 0x6))));
}
