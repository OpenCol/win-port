/* get_p_4_1068_d61a -- shipped at 1068:d61a.
 *
 * Ported by tools/port.py from matched/gfx/get_p_4_1068_d61a.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_4_1068_d61a(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x4)));
}
