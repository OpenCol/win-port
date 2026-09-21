/* GRPort_IsLocked -- shipped at 1068:9729.
 *
 * Ported by tools/port.py from matched/gfx/GRPort_IsLocked.c.
 *
 * The file's own GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char GRPort_IsLocked(GRPort *p)
{
    return (*((void * *)((unsigned char *)(p) + 0x18))) == 0 ? 0 : 1;
}
