/* get_p_6_1068_96e8 -- shipped at 1068:96e8.
 *
 * Ported by tools/port.py from matched/gfx/get_p_6_1068_96e8.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_6_1068_96e8(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x6)));
}
