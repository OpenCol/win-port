/* get_p_8 -- shipped at 1068:d637.
 *
 * Ported by tools/port.py from matched/gfx/get_p_8.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_8(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x8)));
}
