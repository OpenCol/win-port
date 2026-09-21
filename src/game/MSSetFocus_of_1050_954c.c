/* MSSetFocus_of_1050_954c -- shipped at 1050:954c.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1050_954c.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSSetFocus(void *w);

void MSSetFocus_of_1050_954c(Obj *o)
{
    MSSetFocus((*((void * *)((unsigned char *)(o) + 0x6))));
}
