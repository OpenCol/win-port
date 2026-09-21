/* MSSetFocus_of_1038_e53e -- shipped at 1038:e53e.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1038_e53e.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSSetFocus(void *w);

void MSSetFocus_of_1038_e53e(Obj *o)
{
    MSSetFocus((*((void * *)((unsigned char *)(o) + 0x6))));
}
