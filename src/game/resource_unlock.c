/* resource_unlock -- shipped at 1040:6bcf.
 *
 * Ported by tools/port.py from matched/game/resource_unlock.c.
 *
 * The file's own Resource dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSHUnlock(col_i16 h);

void resource_unlock(Resource *r)
{
    if ((*((void * *)((unsigned char *)(r) + 0x4))) != 0) {
        MSHUnlock((*((col_i16 *)((unsigned char *)(r) + 0x2))));
        (*((void * *)((unsigned char *)(r) + 0x4))) = 0;
    }
}
