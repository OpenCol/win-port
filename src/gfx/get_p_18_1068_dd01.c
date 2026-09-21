/* get_p_18_1068_dd01 -- shipped at 1068:dd01.
 *
 * Ported by tools/port.py from matched/gfx/get_p_18_1068_dd01.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * get_p_18_1068_dd01(Rec *r)
{
    return (*((void * *)((unsigned char *)(r) + 0x18)));
}
